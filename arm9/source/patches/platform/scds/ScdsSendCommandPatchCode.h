#pragma once
#include "sections.h"
#include "thumbInstructions.h"

DEFINE_SECTION_SYMBOLS(scds_sendcommand);

extern "C" void scds_sendCommand(u32 command);

class ScdsSendCommandPatchCode : public PatchCode
{
public:
    explicit ScdsSendCommandPatchCode(PatchHeap& patchHeap)
        : PatchCode(SECTION_START(scds_sendcommand), SECTION_SIZE(scds_sendcommand), patchHeap) { }

    const void* GetSendCommandFunction() const
    {
        return GetAddressAtTarget((void*)scds_sendCommand);
    }
};
