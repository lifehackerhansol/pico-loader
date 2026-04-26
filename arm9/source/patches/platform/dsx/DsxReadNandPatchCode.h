#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsPatchCode.h"
#include "DsxSetupCommandPatchCode.h"
#include "DsxWaitBusyPatchCode.h"
#include "DsxZoneSwitchPatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_readnand);

extern "C" void dsx_readNand(u32 srcSector, void* dst, u32 sectorCount);

extern u32 dsx_readNand_waitBusy_address;
extern u32 dsx_readNand_zoneSwitch_address;
extern u32 dsx_readNand_setupCommand_address;

class DsxReadNandPatchCode : public PatchCode, public IReadSectorsPatchCode
{
public:
    explicit DsxReadNandPatchCode(PatchHeap& patchHeap,
        const DsxSetupCommandPatchCode* dsxSetupCommandPatchCode,
        const DsxWaitBusyPatchCode* dsxWaitBusyPatchCode,
        const DsxZoneSwitchPatchCode* dsxZoneSwitchPatchCode)
        : PatchCode(SECTION_START(dsx_readnand), SECTION_SIZE(dsx_readnand), patchHeap) {
            dsx_readNand_setupCommand_address = (u32)dsxSetupCommandPatchCode->GetSetupCommandFunction();
            dsx_readNand_waitBusy_address = (u32)dsxWaitBusyPatchCode->GetWaitBusyFunction();
            dsx_readNand_zoneSwitch_address = (u32)dsxZoneSwitchPatchCode->GetZoneSwitchFunction();
        }

    const ReadSectorsFunc GetReadSectorsFunction() const override
    {
        return (const ReadSectorsFunc)GetAddressAtTarget((void*)dsx_readNand);
    }
};
