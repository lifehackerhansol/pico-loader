#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IWriteSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(ezp_writesave);

extern "C" void ezp_writeSave(u32 dstSector, const void* src);

class EzpWriteSavePatchCode : public PatchCode, public IWriteSectorsPatchCode
{
public:
    explicit EzpWriteSavePatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(ezp_writesave), SECTION_SIZE(ezp_writesave), patchHeap)
    { }

    const WriteSectorsFunc GetWriteSectorFunction() const override
    {
        return (const WriteSectorsFunc)GetAddressAtTarget((void*)ezp_writeSave);
    }
};
