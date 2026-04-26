#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "DsxSetupCommandPatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_zoneswitch);

extern "C" void dsx_zoneSwitch(void);
extern u32 dsx_zoneSwitch_setupCommand_address;

class DsxZoneSwitchPatchCode : public PatchCode
{
public:
    explicit DsxZoneSwitchPatchCode(PatchHeap& patchHeap,
        const DsxSetupCommandPatchCode* dsxSetupCommandPatchCode)
        : PatchCode(SECTION_START(dsx_zoneswitch), SECTION_SIZE(dsx_zoneswitch), patchHeap) {
            dsx_zoneSwitch_setupCommand_address = (u32)dsxSetupCommandPatchCode->GetSetupCommandFunction();
        }

    const void* GetZoneSwitchFunction() const
    {
        return GetAddressAtTarget((void*)dsx_zoneSwitch);
    }
};
