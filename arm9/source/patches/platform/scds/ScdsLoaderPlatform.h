#pragma once
#include "common.h"
#include "../LoaderPlatform.h"
#include "ScdsReadSdPatchCode.h"
#include "ScdsReadSectorLoopPatchCode.h"
#include "ScdsSendCommandPatchCode.h"
#include "ScdsSendSdioCommandPatchCode.h"
#include "ScdsWriteSdPatchCode.h"
#include "ScdsWriteSingleBlockPatchCode.h"

/// @brief Implementation of LoaderPlatform for the GMP-Z003 flashcard
class ScdsLoaderPlatform : public LoaderPlatform
{
public:
    const IReadSectorsPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            const ScdsSendCommandPatchCode* sendCommandPatchCode = 
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsSendCommandPatchCode(patchHeap);
                });

            return new ScdsReadSdPatchCode(patchHeap,
                sendCommandPatchCode,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsSendSDIOCommandPatchCode(patchHeap, sendCommandPatchCode);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsReadSectorLoopPatchCode(patchHeap, sendCommandPatchCode);
                }));
        });
    }

    const IWriteSectorsPatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            const ScdsSendCommandPatchCode* sendCommandPatchCode = 
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsSendCommandPatchCode(patchHeap);
                });

            return new ScdsWriteSdPatchCode(patchHeap,
                sendCommandPatchCode,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsSendSDIOCommandPatchCode(patchHeap, sendCommandPatchCode);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsWriteSingleBlockPatchCode(patchHeap, sendCommandPatchCode);
                }));
        });
    }

    LoaderPlatformType GetPlatformType() const override { return LoaderPlatformType::Slot1; }

};
