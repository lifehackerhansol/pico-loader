#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(ezp_readsave);

extern "C" void ezp_readSave(u32 srcSector, void* dst);

class EzpReadSavePatchCode : public PatchCode, public IReadSectorsPatchCode
{
public:
    explicit EzpReadSavePatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(ezp_readsave), SECTION_SIZE(ezp_readsave), patchHeap)
    { }

    const ReadSectorsFunc GetReadSectorsFunction() const override
    {
        return (const ReadSectorsFunc)GetAddressAtTarget((void*)ezp_readSave);
    }
};