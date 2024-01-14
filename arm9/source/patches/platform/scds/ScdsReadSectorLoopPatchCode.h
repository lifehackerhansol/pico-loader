#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "ScdsSendCommandPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_readsectorloop);

extern u32 scds_sendCommand_rdsect_address;

extern "C" void scds_readSectorLoop(u8 cmd, u32 parameter, u8 responseType);

class ScdsReadSectorLoopPatchCode : public PatchCode
{
public:
    explicit ScdsReadSectorLoopPatchCode(PatchHeap& patchHeap, const ScdsSendCommandPatchCode* scdsSendCommandPatchCode)
        : PatchCode(SECTION_START(scds_readsectorloop), SECTION_SIZE(scds_readsectorloop), patchHeap)
        {
            scds_sendCommand_rdsect_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
        }

    const void* GetReadSectorLoopFunction() const
    {
        return GetAddressAtTarget((void*)scds_readSectorLoop);
    }
};
