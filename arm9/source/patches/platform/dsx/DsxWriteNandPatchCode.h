#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IWriteSectorsPatchCode.h"
#include "DsxResetFpgaAddressPatchCode.h"
#include "DsxSetupCommandPatchCode.h"
#include "DsxWaitBusyPatchCode.h"
#include "DsxZoneSwitchPatchCode.h"

DEFINE_SECTION_SYMBOLS(dsx_writenand);
DEFINE_SECTION_SYMBOLS(dsx_writenand_transferblock);
DEFINE_SECTION_SYMBOLS(dsx_writenand_stoptransmission);

extern "C" void dsx_writeNand(u32 dstSector, const void* src, u32 sectorCount);
extern "C" void dsx_writeNand_transferBlock(u32 dstSector, const void* src, u32 sectorCount);
extern "C" void dsx_writeNand_stopTransmission(void);

extern u32 dsx_writeNand_waitBusy_address;
extern u32 dsx_writeNand_resetFpgaAddress_address;
extern u32 dsx_writeNand_zoneSwitch_address;
extern u32 dsx_writeNand_transferBlock_address;
extern u32 dsx_writeNand_stopTransmission_address;
extern u32 dsx_writeNand_setupCommand_address;
extern u32 dsx_writeNand_stopTransmission_waitBusy_address;

class DsxWriteNandStopTransmissionPatchCode : public PatchCode
{
public:
    explicit DsxWriteNandStopTransmissionPatchCode(PatchHeap& patchHeap,
        const DsxWaitBusyPatchCode* dsxWaitBusyPatchCode)
        : PatchCode(SECTION_START(dsx_writenand_stoptransmission), SECTION_SIZE(dsx_writenand_stoptransmission), patchHeap) {
            dsx_writeNand_stopTransmission_waitBusy_address = (u32)dsxWaitBusyPatchCode->GetWaitBusyFunction();
        }

    const void* GetWriteNandStopTransmissionFunction() const
    {
        return GetAddressAtTarget((void*)dsx_writeNand_stopTransmission);
    }
};

class DsxWriteNandTransferBlockPatchCode : public PatchCode
{
public:
    explicit DsxWriteNandTransferBlockPatchCode(PatchHeap& patchHeap,
        const DsxSetupCommandPatchCode* dsxSetupCommandPatchCode)
        : PatchCode(SECTION_START(dsx_writenand_transferblock), SECTION_SIZE(dsx_writenand_transferblock), patchHeap) {
            dsx_writeNand_setupCommand_address = (u32)dsxSetupCommandPatchCode->GetSetupCommandFunction();
        }

    const void* GetWriteNandTransferBlockFunction() const
    {
        return GetAddressAtTarget((void*)dsx_writeNand_transferBlock);
    }
};

class DsxWriteNandPatchCode : public PatchCode, public IWriteSectorsPatchCode
{
public:
    explicit DsxWriteNandPatchCode(PatchHeap& patchHeap,
        const DsxResetFpgaAddressPatchCode* dsxResetFpgaAddressPatchCode,
        const DsxWaitBusyPatchCode* dsxWaitBusyPatchCode,
        const DsxZoneSwitchPatchCode* dsxZoneSwitchPatchCode,
        const DsxWriteNandTransferBlockPatchCode* dsxWriteNandTransferBlockPatchCode,
        const DsxWriteNandStopTransmissionPatchCode* dsxWriteNandStopTransmissionPatchCode
    )
        : PatchCode(SECTION_START(dsx_writenand), SECTION_SIZE(dsx_writenand), patchHeap)
        {
            dsx_writeNand_waitBusy_address = (u32)dsxWaitBusyPatchCode->GetWaitBusyFunction();
            dsx_writeNand_resetFpgaAddress_address = (u32)dsxResetFpgaAddressPatchCode->GetResetFpgaAddressFunction();
            dsx_writeNand_zoneSwitch_address = (u32)dsxZoneSwitchPatchCode->GetZoneSwitchFunction();
            dsx_writeNand_transferBlock_address = (u32)dsxWriteNandTransferBlockPatchCode->GetWriteNandTransferBlockFunction();
            dsx_writeNand_stopTransmission_address = (u32)dsxWriteNandStopTransmissionPatchCode->GetWriteNandStopTransmissionFunction();
        }

    const WriteSectorsFunc GetWriteSectorFunction() const override
    {
        return (const WriteSectorsFunc)GetAddressAtTarget((void*)dsx_writeNand);
    }
};
