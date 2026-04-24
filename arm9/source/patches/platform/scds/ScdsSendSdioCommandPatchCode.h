#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "ScdsSendCommandPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_sendsdiocommand);

extern u32 scds_sendCommand_sdio_address;

extern "C" void scds_sendSdioCommand(u8 cmd, u32 parameter, u8 responseType);

class ScdsSendSdioCommandPatchCode : public PatchCode
{
public:
    explicit ScdsSendSdioCommandPatchCode(PatchHeap& patchHeap, const ScdsSendCommandPatchCode* scdsSendCommandPatchCode)
        : PatchCode(SECTION_START(scds_sendsdiocommand), SECTION_SIZE(scds_sendsdiocommand), patchHeap)
        {
            scds_sendCommand_sdio_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
        }

    const void* GetSendSDIOCommandFunction() const
    {
        return GetAddressAtTarget((void*)scds_sendSdioCommand);
    }
};
