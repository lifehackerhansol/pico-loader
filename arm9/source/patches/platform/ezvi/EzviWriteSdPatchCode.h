#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IWriteSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(ezvi_writesd);

extern "C" void ezvi_writeSd(u32 dstSector, const void* src, u32 sectorCount);

class EzviWriteSdPatchCode : public PatchCode, public IWriteSectorsPatchCode
{
public:
    explicit EzviWriteSdPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(ezvi_writesd), SECTION_SIZE(ezvi_writesd), patchHeap) { }

    const WriteSectorsFunc GetWriteSectorFunction() const override
    {
        return (const WriteSectorsFunc)GetAddressAtTarget((void*)ezvi_writeSd);
    }
};
