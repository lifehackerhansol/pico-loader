#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdReadPatchCode.h"
#include "dgnmSendSdioCommandAsm.h"

DEFINE_SECTION_SYMBOLS(dgnm_readsd);

extern "C" void dgnm_readSd(u32 srcSector, void* dst, u32 sectorCount);

extern u32 dgnm_readSd_sendSdioCommand_address;

class DGNMReadSdPatchCode : public SdReadPatchCode
{
public:
    explicit DGNMReadSdPatchCode(
        PatchHeap& patchHeap,
        const DGNMSendSdioCommandPatchCode* dgnmSendSdioCommandPatchCode
    )
        : SdReadPatchCode(SECTION_START(dgnm_readsd), SECTION_SIZE(dgnm_readsd), patchHeap)
        {
            dgnm_readSd_sendSdioCommand_address = (u32)dgnmSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        }

    const SdReadFunc GetSdReadFunction() const override
    {
        return (const SdReadFunc)GetAddressAtTarget((void*)dgnm_readSd);
    }
};
