#include "common.h"
#include <algorithm>
#include <string.h>
#include "SaveList.h"
#include "SaveListFactory.h"
#include "fileInfo.h"
#include "gameCode.h"
#include "CardSaveArranger.h"

#define SAVE_LIST_PATH      "/_pico/savelist.bin"
#define DEFAULT_SAVE_SIZE   (512 * 1024)
#define SAVE_FILL_VALUE     0xFF
#define NTR_NAND_BLOCK_SIZE 0x20000
#define TWL_NAND_BLOCK_SIZE 0x80000
#define NAND_RW_REGION_END  0x07A00000

static const u8 sBandBrothersSaveId[12] = { 0x48, 0x8A, 0x00, 0x00, 0x42, 0x42, 0x44, 0x58, 0x31, 0x32, 0x33, 0x34 };
static const u8 sJamWithTheBandSaveId[16] = { 0xEC, 0x00, 0x9E, 0xA1, 0x51, 0x65, 0x34, 0x35, 0x30, 0x35, 0x30, 0x31, 0x19, 0x19, 0x02, 0x0A };

bool CardSaveArranger::SetupCardSave(const nds_header_ntr_t* header, const TCHAR* savePath, CardSaveResult& result) const
{
    u32 saveSize = DEFAULT_SAVE_SIZE;
    if (header->nandBackupRegionStart != 0)
    {
        u32 blockSize = header->SupportsDsiMode() ? TWL_NAND_BLOCK_SIZE : NTR_NAND_BLOCK_SIZE;
        u32 nandBackupRegionStart = header->nandBackupRegionStart * blockSize;
        saveSize = NAND_RW_REGION_END - nandBackupRegionStart;
        LOG_DEBUG("NAND save. Size: 0x%X.", saveSize);
    }
    else
    {
        auto saveList = SaveListFactory().CreateFromFile(SAVE_LIST_PATH);
        if (saveList)
        {
            const auto saveListEntry = saveList->FindEntry(header->gameCode);
            if (!saveListEntry)
            {
                LOG_WARNING("Game code %c%c%c%c not found in save list\n",
                    header->gameCode & 0xFF, (header->gameCode >> 8) & 0xFF,
                    (header->gameCode >> 16) & 0xFF, header->gameCode >> 24);
            }
            else
            {
                saveSize = saveListEntry->GetSaveSize();
                saveListEntry->Dump();
            }
        }
    }
    if (saveSize == 0)
    {
        result.hasSave = false;
        return true;
    }

    auto file = std::make_unique<FIL>();
    LOG_DEBUG("Save file: %s\n", savePath);
    if (f_open(file.get(), savePath, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
    {
        LOG_FATAL("Failed to open or create save file\n");
        return false;
    }
    u32 initialSize = f_size(file.get());
    if (initialSize < saveSize)
    {
        if (f_lseek(file.get(), saveSize) != FR_OK ||
            f_lseek(file.get(), initialSize) != FR_OK)
        {
            LOG_FATAL("Failed to expand save file\n");
            return false;
        }

        const u32 ffBufferSize = 32 * 1024;
        auto ffBuffer = std::make_unique<u8[]>(ffBufferSize);
        memset(ffBuffer.get(), SAVE_FILL_VALUE, ffBufferSize);

        u32 offset = initialSize;
        // Align to 512 bytes
        if ((offset & 511) != 0)
        {
            u32 remainingTo512 = 512 - (offset & 511);
            UINT bytesWritten = 0;
            if (f_write(file.get(), ffBuffer.get(), remainingTo512, &bytesWritten) != FR_OK ||
                bytesWritten != remainingTo512)
            {
                LOG_FATAL("Failed to expand save file\n");
                return false;
            }
            offset += remainingTo512;
        }

        // Write in up to 32kb blocks
        while (offset < saveSize)
        {
            u32 bytesToWrite = std::min<u32>(saveSize - offset, ffBufferSize);
            UINT bytesWritten = 0;
            if (f_write(file.get(), ffBuffer.get(), bytesToWrite, &bytesWritten) != FR_OK ||
                bytesWritten != bytesToWrite)
            {
                LOG_FATAL("Failed to expand save file\n");
                return false;
            }
            offset += bytesToWrite;
        }

        // Additional save initialization
        if (header->gameCode == GAMECODE("AXBJ"))
        {
            // Write save id for Band Brothers
            UINT bytesWritten = 0;
            if (f_lseek(file.get(), 0) != FR_OK ||
                f_write(file.get(), sBandBrothersSaveId, sizeof(sBandBrothersSaveId), &bytesWritten) != FR_OK ||
                bytesWritten != sizeof(sBandBrothersSaveId))
            {
                LOG_FATAL("Failed to write Band Brothers save id\n");
                return false;
            }
        }
        else if (header->gameCode == GAMECODE("UXBP"))
        {
            // Write save id for Jam with the Band
            // See also https://github.com/melonDS-emu/melonDS/blob/3a3388c4c50e8735af125c1af4d89e457f5e9035/src/NDSCart.cpp#L1067
            UINT bytesWritten = 0;
            if (f_lseek(file.get(), saveSize - 0x800) != FR_OK ||
                f_write(file.get(), sJamWithTheBandSaveId, sizeof(sJamWithTheBandSaveId), &bytesWritten) != FR_OK ||
                bytesWritten != sizeof(sJamWithTheBandSaveId))
            {
                LOG_FATAL("Failed to write Jam with the Band save id\n");
                return false;
            }
        }
    }

    DWORD* clusterTab = (DWORD*)SHARED_SAVE_FILE_INFO->clusterMap;
    clusterTab[0] = sizeof(SHARED_SAVE_FILE_INFO->clusterMap) / sizeof(u32);
    file->cltbl = clusterTab;
    FRESULT seekResult = f_lseek(file.get(), CREATE_LINKMAP);
    if (seekResult != FR_OK)
    {
        LOG_FATAL("Failed to make save cluster table. Result: %d\n", seekResult);
        return false;
    }
    SHARED_SAVE_FILE_INFO->clusterShift = __builtin_ctz(file->obj.fs->csize);
    SHARED_SAVE_FILE_INFO->database = file->obj.fs->database;
    SHARED_SAVE_FILE_INFO->clusterMask = file->obj.fs->csize - 1;

    LOG_DEBUG("Made save cluster table\n");

    result.saveDirSector = file->dir_sect;
    result.saveDirSectorOffset = (u32)(file->dir_ptr - file->obj.fs->win);

    if (f_close(file.get()) != FR_OK)
    {
        LOG_FATAL("Failed to close save file\n");
        return false;
    }

    result.hasSave = true;
    return true;
}
