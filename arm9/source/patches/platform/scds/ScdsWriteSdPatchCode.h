#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "patches/PatchCode.h"
#include "ScdsSendCommandPatchCode.h"
#include "ScdsSendSdioCommandPatchCode.h"
#include "ScdsWriteSingleBlockPatchCode.h"
#include "../IWriteSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_writesd);

extern u32 scds_sendCommand_wr_address;
extern u32 scds_sendSdioCommand_wr_address;
extern u32 scds_writeSingleBlock_address;

extern "C" void scds_writeSd(u32 dstSector, const void* src, u32 sectorCount);

class ScdsWriteSdPatchCode : public PatchCode, public IWriteSectorsPatchCode
{
public:
    explicit ScdsWriteSdPatchCode(PatchHeap& patchHeap,
                                 const ScdsSendCommandPatchCode* scdsSendCommandPatchCode,
                                 const ScdsSendSdioCommandPatchCode* scdsSendSdioCommandPatchCode,
                                 const ScdsWriteSingleBlockPatchCode* scdsWriteSingleBlockPatchCode
                                )
        : PatchCode(SECTION_START(scds_writesd), SECTION_SIZE(scds_writesd), patchHeap) {
            scds_sendCommand_wr_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
            scds_sendSdioCommand_wr_address = (u32)scdsSendSdioCommandPatchCode->GetSendSDIOCommandFunction();
            scds_writeSingleBlock_address = (u32)scdsWriteSingleBlockPatchCode->GetWriteSingleBlockFunction();
        }

    const WriteSectorsFunc GetWriteSectorFunction() const override
    {
        return (const WriteSectorsFunc)GetAddressAtTarget((void*)scds_writeSd);
    }
};
