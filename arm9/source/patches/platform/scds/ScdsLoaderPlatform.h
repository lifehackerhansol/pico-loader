#pragma once
#include "common.h"
#include "../LoaderPlatform.h"
#include "ScdsReadSdDmaPatchCode.h"
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
        const ScdsSendCommandPatchCode* sendCommandPatchCode = 
            patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ScdsSendCommandPatchCode(patchHeap);
            });
        const ScdsSendSdioCommandPatchCode* sendSdioCommandPatchCode = 
            patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ScdsSendSdioCommandPatchCode(patchHeap, sendCommandPatchCode);
            });
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new ScdsReadSdPatchCode(patchHeap,
                sendSdioCommandPatchCode,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsReadSectorLoopPatchCode(
                        patchHeap,
                        sendCommandPatchCode
                    );
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsSdStopTransmissionPatchCode(
                        patchHeap,
                        sendCommandPatchCode,
                        sendSdioCommandPatchCode
                    );
                })
            );
        });
    }

    const IReadSectorsDmaPatchCode* CreateSdReadDmaPatchCode(PatchCodeCollection& patchCodeCollection,
        PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr) const override
    {
        const ScdsSendCommandPatchCode* sendCommandPatchCode = 
            patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ScdsSendCommandPatchCode(patchHeap);
            });
        const ScdsSendSdioCommandPatchCode* sendSdioCommandPatchCode = 
            patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ScdsSendSdioCommandPatchCode(patchHeap, sendCommandPatchCode);
            });
        return patchCodeCollection.AddUniquePatchCode<ScdsReadSdDmaPatchCode>(
            patchHeap, miiCardDmaCopy32Ptr,
            sendCommandPatchCode,
            sendSdioCommandPatchCode,
            patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new ScdsSdStopTransmissionPatchCode(
                    patchHeap,
                    sendCommandPatchCode,
                    sendSdioCommandPatchCode
                );
            })
        );
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
                    return new ScdsSendSdioCommandPatchCode(patchHeap, sendCommandPatchCode);
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new ScdsWriteSingleBlockPatchCode(patchHeap, sendCommandPatchCode);
                }));
        });
    }

    LoaderPlatformType GetPlatformType() const override { return LoaderPlatformType::Slot1; }

    bool HasDmaSdReads() const override { return true; }
};
