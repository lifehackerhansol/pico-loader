#pragma once
#include "sections.h"
#include "thumbInstructions.h"

DEFINE_SECTION_SYMBOLS(dgnm_sendsdiocommand);

extern "C" void dgnm_sendSdioCommand(u32 srcSector, void* dst, u32 sectorCount);

class DGNMSendSdioCommandPatchCode : public PatchCode
{
public:
    explicit DGNMSendSdioCommandPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(dgnm_sendsdiocommand), SECTION_SIZE(dgnm_sendsdiocommand), patchHeap) { }

    const void* GetSendSdioCommandFunction() const
    {
        return GetAddressAtTarget((void*)dgnm_sendSdioCommand);
    }
};
