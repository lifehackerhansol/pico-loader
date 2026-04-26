#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "DsxSetupCommandPatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_resetfpgaaddress);

extern "C" void dsx_resetFpgaAddress(void);

class DsxResetFpgaAddressPatchCode : public PatchCode
{
public:
    explicit DsxResetFpgaAddressPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(dsx_resetfpgaaddress), SECTION_SIZE(dsx_resetfpgaaddress), patchHeap) { }

    const void* GetResetFpgaAddressFunction() const
    {
        return GetAddressAtTarget((void*)dsx_resetFpgaAddress);
    }
};
