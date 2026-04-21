#include "common.h"
#include <fileInfo.h>
#include <libtwl/card/card.h>
#include "EzpLoaderPlatform.h"

#define EZP_CMD_CARD_INFO    0xB800000000000000ull
#define EZP_CMD_SEND_MAP     0xB100000100000000ull
#define EZP_FLAGS_CARD_PARAM (MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(24) | MCCNT1_LEN_4)
#define EZP_FLAGS_WRITE_512 (MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_DIR_WRITE | MCCNT1_LATENCY2(26) | MCCNT1_LATENCY1(0x1FFF) | MCCNT1_LEN_512)


u32 EzpLoaderPlatform::ReadCardInfo(void) const
{
    card_romSetCmd(EZP_CMD_CARD_INFO);
    card_romStartXfer(EZP_FLAGS_CARD_PARAM | MCCNT1_LATENCY1(0xC8), false);
    card_romWaitDataReady();
    return card_romGetData();
}

void EzpLoaderPlatform::PrepareRomBoot(u32 romDirSector, u32 romDirSectorOffset, u32 saveDirSector, u32 saveDirSectorOffset) const
{
    u32 ezpClusterMap[128] = {0};
    u32 clusterMapIdx = 0;

    u32 fileOffset = 0;
    for (u32 i=0; i < (SHARED_ROM_FILE_INFO->clusterMap[0] - 2); i+=2)
    {
        u32 sector = (SHARED_ROM_FILE_INFO->clusterMap[i + 2] - 2);
        sector *= (1 << SHARED_ROM_FILE_INFO->clusterShift);
        sector += SHARED_ROM_FILE_INFO->database;
        ezpClusterMap[++clusterMapIdx] = sector;

        fileOffset += (SHARED_ROM_FILE_INFO->clusterMap[i + 1] << SHARED_ROM_FILE_INFO->clusterShift);
        ezpClusterMap[++clusterMapIdx] = fileOffset;
    }
    ezpClusterMap[clusterMapIdx] = 0xFFFFFFFF;

    clusterMapIdx = 64;
    fileOffset = 0;
    for (u32 i=0; i < (SHARED_SAVE_FILE_INFO->clusterMap[0] - 2); i+=2)
    {
        u32 sector = (SHARED_SAVE_FILE_INFO->clusterMap[i + 2] - 2);
        sector *= (1 << SHARED_SAVE_FILE_INFO->clusterShift);
        sector += SHARED_SAVE_FILE_INFO->database;

        ezpClusterMap[++clusterMapIdx] = sector;

        fileOffset += (SHARED_SAVE_FILE_INFO->clusterMap[i + 1] << SHARED_SAVE_FILE_INFO->clusterShift);
        ezpClusterMap[++clusterMapIdx] = fileOffset;
    }
    ezpClusterMap[clusterMapIdx] = 0xFFFFFFFF;


    ReadCardInfo();

    card_romSetCmd(EZP_CMD_SEND_MAP);
    card_romStartXfer(EZP_FLAGS_WRITE_512, false);
    card_romCpuWrite(ezpClusterMap, 128);

    ReadCardInfo();
}
