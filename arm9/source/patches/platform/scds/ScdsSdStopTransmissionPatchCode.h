#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "ScdsSendCommandPatchCode.h"
#include "ScdsSendSdioCommandPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_sdstoptransmission);

extern u32 scds_sendCommand_cmd12_address;
extern u32 scds_sendSdioCommand_cmd12_address;

extern "C" void scds_sdStopTransmission(void);

class ScdsSdStopTransmissionPatchCode : public PatchCode
{
public:
    explicit ScdsSdStopTransmissionPatchCode(PatchHeap& patchHeap,
        const ScdsSendCommandPatchCode* scdsSendCommandPatchCode,
        const ScdsSendSdioCommandPatchCode* scdsSendSdioCommandPatchCode)
        : PatchCode(SECTION_START(scds_sdstoptransmission), SECTION_SIZE(scds_sdstoptransmission), patchHeap)
        {
            scds_sendCommand_cmd12_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
            scds_sendSdioCommand_cmd12_address = (u32)scdsSendSdioCommandPatchCode->GetSendSDIOCommandFunction();
        }

    const void* GetSdStopTransmissionFunction() const
    {
        return GetAddressAtTarget((void*)scds_sdStopTransmission);
    }
};
