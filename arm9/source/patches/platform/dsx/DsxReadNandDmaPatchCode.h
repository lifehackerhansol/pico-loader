#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsDmaPatchCode.h"
#include "DsxSetupCommandPatchCode.h"
#include "DsxWaitBusyPatchCode.h"
#include "DsxZoneSwitchPatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_readnanddma);

extern "C" void dsx_readNandDma(u32 srcSector, u32 previousSrcSector, u32 dmaChannel, void* dst);
extern "C" void dsx_finishReadNandDma(void);

extern u32 dsx_readNandDma_miiCardDmaCopy32Ptr;

extern u32 dsx_readNandDma_waitBusy_address;
extern u32 dsx_readNandDma_zoneSwitch_address;
extern u32 dsx_readNandDma_setupCommand_address;

class DsxReadNandDmaPatchCode : public PatchCode, public IReadSectorsDmaPatchCode
{
public:
    DsxReadNandDmaPatchCode(PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr,
        const DsxSetupCommandPatchCode* dsxSetupCommandPatchCode,
        const DsxWaitBusyPatchCode* dsxWaitBusyPatchCode,
        const DsxZoneSwitchPatchCode* dsxZoneSwitchPatchCode
        )
        : PatchCode(SECTION_START(dsx_readnanddma), SECTION_SIZE(dsx_readnanddma), patchHeap)
    {
        dsx_readNandDma_miiCardDmaCopy32Ptr = (u32)miiCardDmaCopy32Ptr;
        dsx_readNandDma_setupCommand_address = (u32)dsxSetupCommandPatchCode->GetSetupCommandFunction();
        dsx_readNandDma_waitBusy_address = (u32)dsxWaitBusyPatchCode->GetWaitBusyFunction();
        dsx_readNandDma_zoneSwitch_address = (u32)dsxZoneSwitchPatchCode->GetZoneSwitchFunction();
    }

    const ReadSectorsDmaFunc GetReadSectorsDmaFunction() const override
    {
        return (const ReadSectorsDmaFunc)GetAddressAtTarget((void*)dsx_readNandDma);
    }

    const ReadSectorsDmaFinishFunc GetReadSectorsDmaFinishFunction() const override
    {
        return (const ReadSectorsDmaFinishFunc)GetAddressAtTarget((void*)dsx_finishReadNandDma);
    }
};
