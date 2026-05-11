#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsDmaPatchCode.h"
#include "EzviApplySectorPatchCode.h"

DEFINE_SECTION_SYMBOLS(ezvi_readsddma);

extern "C" void ezvi_readSdDma(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);
extern "C" void ezvi_finishReadSdDma(void);

extern u32 ezvi_readSdDma_miiCardDmaCopy32Ptr;
extern u32 ezvi_readSdDma_applySector_address;

class EzviReadSdDmaPatchCode : public PatchCode, public IReadSectorsDmaPatchCode
{
public:
    EzviReadSdDmaPatchCode(PatchHeap& patchHeap,
        const void* miiCardDmaCopy32Ptr,
        const EzviApplySectorPatchCode* applySectorPatchCode
    )
        : PatchCode(SECTION_START(ezvi_readsddma), SECTION_SIZE(ezvi_readsddma), patchHeap)
    {
        ezvi_readSdDma_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
        ezvi_readSdDma_applySector_address = (u32)applySectorPatchCode->GetApplySectorFunction();
    }

    const ReadSectorsDmaFunc GetReadSectorsDmaFunction() const override
    {
        return (const ReadSectorsDmaFunc)GetAddressAtTarget((void*)ezvi_readSdDma);
    }

    const ReadSectorsDmaFinishFunc GetReadSectorsDmaFinishFunction() const override
    {
        return (const ReadSectorsDmaFinishFunc)GetAddressAtTarget((void*)ezvi_finishReadSdDma);
    }
};
