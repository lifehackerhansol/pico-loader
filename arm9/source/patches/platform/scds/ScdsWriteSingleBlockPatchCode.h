#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "ScdsSendCommandPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_writesingleblock);

extern u32 scds_sendCommand_wrsb_address;

extern "C" void scds_writeSingleBlock(u32 sector, u32* dest, u32 count);

class ScdsWriteSingleBlockPatchCode : public PatchCode
{
public:
    explicit ScdsWriteSingleBlockPatchCode(PatchHeap& patchHeap, const ScdsSendCommandPatchCode* scdsSendCommandPatchCode)
        : PatchCode(SECTION_START(scds_writesingleblock), SECTION_SIZE(scds_writesingleblock), patchHeap)
        {
            scds_sendCommand_wrsb_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
        }

    const void* GetWriteSingleBlockFunction() const
    {
        return GetAddressAtTarget((void*)scds_writeSingleBlock);
    }
};
