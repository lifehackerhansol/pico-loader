#pragma once
#include "sections.h"
#include "patches/PatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_waitbusy);

extern "C" void dsx_waitBusy(void);

class DsxWaitBusyPatchCode : public PatchCode
{
public:
    explicit DsxWaitBusyPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(dsx_waitbusy), SECTION_SIZE(dsx_waitbusy), patchHeap) { }

    const void* GetWaitBusyFunction() const
    {
        return GetAddressAtTarget((void*)dsx_waitBusy);
    }
};
