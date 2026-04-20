#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(r4_readromdma);

extern "C" void r4_readRomDma(u32 srcSector, void* dst);
extern "C" void r4_finishReadRomDma(void);

extern u32 r4_readRomDma_miiCardDmaCopy32Ptr;

class R4ReadRomDmaPatchCode : public PatchCode, public IReadSectorsDmaPatchCode
{
public:
    explicit R4ReadRomDmaPatchCode(PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr)
        : PatchCode(SECTION_START(r4_readromdma), SECTION_SIZE(r4_readromdma), patchHeap)
    {
        r4_readRomDma_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
    }

    const ReadSectorsDmaFunc GetReadSectorsDmaFunction() const override
    {
        return (const ReadSectorsDmaFunc)GetAddressAtTarget((void*)r4_readRomDma);
    }

    const ReadSectorsDmaFinishFunc GetReadSectorsDmaFinishFunction() const override
    {
        return (const ReadSectorsDmaFinishFunc)GetAddressAtTarget((void*)r4_finishReadRomDma);
    }
};
