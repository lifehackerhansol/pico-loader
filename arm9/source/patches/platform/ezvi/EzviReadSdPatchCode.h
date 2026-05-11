#pragma once
#include "sections.h"
#include "patches/PatchCode.h"
#include "../IReadSectorsPatchCode.h"
#include "EzviApplySectorPatchCode.h"

DEFINE_SECTION_SYMBOLS(ezvi_readsd);
DEFINE_SECTION_SYMBOLS(ezvi_readsd_transferdata);

extern "C" void ezvi_readSd(u32 srcSector, void* dst, u32 sectorCount);
extern u32 ezvi_readSd_applySector_address;

class EzviReadSdPatchCode : public PatchCode, public IReadSectorsPatchCode
{
public:
    explicit EzviReadSdPatchCode(PatchHeap& patchHeap,
        const EzviApplySectorPatchCode* applySectorPatchCode)
        : PatchCode(SECTION_START(ezvi_readsd), SECTION_SIZE(ezvi_readsd), patchHeap)
        {
            ezvi_readSd_applySector_address = (u32)applySectorPatchCode->GetApplySectorFunction();
        }

    const ReadSectorsFunc GetReadSectorsFunction() const override
    {
        return (const ReadSectorsFunc)GetAddressAtTarget((void*)ezvi_readSd);
    }
};
