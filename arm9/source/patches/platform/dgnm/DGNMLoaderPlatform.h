#pragma once
#include "common.h"
#include <libtwl/card/card.h>
#include "../LoaderPlatform.h"
#include "dgnmReadSdAsm.h"
#include "dgnmWriteSdAsm.h"
#include "dgnmSendSdioCommandAsm.h"

/// @brief Implementation of LoaderPlatform for the Datel Games 'n' Music flashcard
class DGNMLoaderPlatform : public LoaderPlatform
{
public:
    const SdReadPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new DGNMReadSdPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new DGNMSendSdioCommandPatchCode(patchHeap);
                }));
        });
    }

    const SdWritePatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new DGNMWriteSdPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new DGNMSendSdioCommandPatchCode(patchHeap);
                }));
        });
    }

    void PrepareRomBoot(
        u32 romDirSector, u32 romDirSectorOffset) const override;

};

