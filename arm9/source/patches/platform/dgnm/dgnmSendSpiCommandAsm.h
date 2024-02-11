#pragma once
#include "sections.h"
#include "thumbInstructions.h"

DEFINE_SECTION_SYMBOLS(dgnm_sendspicommand);

extern "C" void dgnm_sendSpiCommand(u32 srcSector, void* dst, u32 sectorCount);

class DGNMSendSpiCommandPatchCode : public PatchCode
{
public:
    explicit DGNMSendSpiCommandPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(dgnm_sendspicommand), SECTION_SIZE(dgnm_sendspicommand), patchHeap) { }

    const void* GetSendSpiCommandFunction() const
    {
        return GetAddressAtTarget((void*)dgnm_sendSpiCommand);
    }
};
