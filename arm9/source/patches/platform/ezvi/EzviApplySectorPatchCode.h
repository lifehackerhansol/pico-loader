#pragma once
#include "sections.h"
#include "patches/PatchCode.h"

DEFINE_SECTION_SYMBOLS(ezvi_applysector);

extern "C" void ezvi_applySector(u32 sector);

class EzviApplySectorPatchCode : public PatchCode
{
public:
    explicit EzviApplySectorPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(ezvi_applysector), SECTION_SIZE(ezvi_applysector), patchHeap) { }

    const void* GetApplySectorFunction() const
    {
        return GetAddressAtTarget((void*)ezvi_applySector);
    }
};
