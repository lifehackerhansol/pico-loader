#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(ezp_readrom);

extern "C" void ezp_readRom(u32 srcSector, void* dst);

class EzpReadRomPatchCode : public PatchCode, public IReadSectorsPatchCode
{
public:
    explicit EzpReadRomPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(ezp_readrom), SECTION_SIZE(ezp_readrom), patchHeap)
    { }

    const ReadSectorsFunc GetReadSectorsFunction() const override
    {
        return (const ReadSectorsFunc)GetAddressAtTarget((void*)ezp_readRom);
    }
};
