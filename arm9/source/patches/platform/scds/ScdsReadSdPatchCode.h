#pragma once
#include "sections.h"
#include "thumbInstructions.h"
#include "patches/PatchCode.h"
#include "ScdsSendCommandPatchCode.h"
#include "ScdsSendSdioCommandPatchCode.h"
#include "ScdsReadSectorLoopPatchCode.h"
#include "../IReadSectorsPatchCode.h"

DEFINE_SECTION_SYMBOLS(scds_readsd);

extern u32 scds_sendCommand_rd_address;
extern u32 scds_sendSdioCommand_rd_address;
extern u32 scds_readSectorLoop_address;

extern "C" void scds_readSd(u32 srcSector, void* dst, u32 sectorCount);

class ScdsReadSdPatchCode : public PatchCode, public IReadSectorsPatchCode
{
public:
    explicit ScdsReadSdPatchCode(PatchHeap& patchHeap,
                                 const ScdsSendCommandPatchCode* scdsSendCommandPatchCode,
                                 const ScdsSendSDIOCommandPatchCode* scdsSendSdioCommandPatchCode,
                                 const ScdsReadSectorLoopPatchCode* scdsReadSectorLoopPatchCode
                                )
        : PatchCode(SECTION_START(scds_readsd), SECTION_SIZE(scds_readsd), patchHeap)
        {
            scds_sendCommand_rd_address = (u32)scdsSendCommandPatchCode->GetSendCommandFunction();
            scds_sendSdioCommand_rd_address = (u32)scdsSendSdioCommandPatchCode->GetSendSDIOCommandFunction();
            scds_readSectorLoop_address = (u32)scdsReadSectorLoopPatchCode->GetReadSectorLoopFunction();
        }

    const ReadSectorsFunc GetReadSectorsFunction() const override
    {
        return (const ReadSectorsFunc)GetAddressAtTarget((void*)scds_readSd);
    }
};
