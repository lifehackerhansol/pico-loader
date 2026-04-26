#pragma once
#include "sections.h"
#include "patches/PatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_setupcommand);

extern "C" void dsx_setupCommand(u8 cmd, u32 param);

class DsxSetupCommandPatchCode : public PatchCode
{
public:
    explicit DsxSetupCommandPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(dsx_setupcommand), SECTION_SIZE(dsx_setupcommand), patchHeap) { }

    const void* GetSetupCommandFunction() const
    {
        return GetAddressAtTarget((void*)dsx_setupCommand);
    }
};
