#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "patches/PatchCode.h"
#include "ScdsSdStopTransmissionPatchCode.h"
#include "ScdsSendCommandPatchCode.h"
#include "ScdsSendSdioCommandPatchCode.h"
#include "../IReadSectorsDmaPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_readsddma);

extern u32 scds_sendCommand_dma_address;
extern u32 scds_sendSdioCommand_dma_address;
extern u32 scds_miiCardDmaCopy32Ptr;

extern "C" void scds_readSdDma(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);
extern "C" void scds_finishReadSdDma(void);

class ScdsReadSdDmaPatchCode : public PatchCode, public IReadSectorsDmaPatchCode
{
public:
    explicit ScdsReadSdDmaPatchCode(PatchHeap& patchHeap,
                                 const void* miiCardDmaCopy32Ptr,
                                 const ScdsSendCommandPatchCode* scdsSendCommandPatchCode,
                                 const ScdsSendSdioCommandPatchCode* scdsSendSdioCommandPatchCode,
                                 const ScdsSdStopTransmissionPatchCode* scdsSdStopTransmissionPatchCode
                                )
        : PatchCode(SECTION_START(scds_readsddma), SECTION_SIZE(scds_readsddma), patchHeap)
        {
            scds_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
            scds_sendCommand_dma_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
            scds_sendSdioCommand_dma_address = (u32)scdsSendSdioCommandPatchCode->GetSendSDIOCommandFunction();
        }

    const ReadSectorsDmaFunc GetReadSectorsDmaFunction() const override
    {
        return (const ReadSectorsDmaFunc)GetAddressAtTarget((void*)scds_readSdDma);
    }

    const ReadSectorsDmaFinishFunc GetReadSectorsDmaFinishFunction() const override
    {
        return (const ReadSectorsDmaFinishFunc)GetAddressAtTarget((void*)scds_finishReadSdDma);
    }
};
