#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "../SdWritePatchCode.h"
#include "dgnmSendSdioCommandAsm.h"

DEFINE_SECTION_SYMBOLS(dgnm_writesd);

extern "C" void dgnm_writeSd(u32 dstSector, const void* src, u32 sectorCount);

extern u32 dgnm_writeSd_sendSdioCommand_address;

class DGNMWriteSdPatchCode : public SdWritePatchCode
{
public:
    explicit DGNMWriteSdPatchCode(
        PatchHeap& patchHeap,
        const DGNMSendSdioCommandPatchCode* dgnmSendSdioCommandPatchCode
    )
        : SdWritePatchCode(SECTION_START(dgnm_writesd), SECTION_SIZE(dgnm_writesd), patchHeap)
        {
            dgnm_writeSd_sendSdioCommand_address = (u32)dgnmSendSdioCommandPatchCode->GetSendSdioCommandFunction();
        }

    const SdWriteFunc GetSdWriteFunction() const override
    {
        return (const SdWriteFunc)GetAddressAtTarget((void*)dgnm_writeSd);
    }
};
