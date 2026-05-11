#pragma once
#include "../LoaderPlatform.h"
#include "EzviApplySectorPatchCode.h"
#include "EzviReadSdPatchCode.h"
#include "EzviReadSdDmaPatchCode.h"
#include "EzviWriteSdPatchCode.h"

/// @brief Implementation of LoaderPlatform for the EZ-Flash Vi flashcard
class EzviLoaderPlatform : public LoaderPlatform
{
public:
    const IReadSectorsPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzviReadSdPatchCode(
                patchHeap,
                CreateApplySectorPatchCode(patchCodeCollection, patchHeap)
            );
        });
    }

    const IReadSectorsDmaPatchCode* CreateSdReadDmaPatchCode(PatchCodeCollection& patchCodeCollection,
        PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr) const override
    {
        return patchCodeCollection.AddUniquePatchCode<EzviReadSdDmaPatchCode>(
            patchHeap,
            miiCardDmaCopy32Ptr,
            CreateApplySectorPatchCode(patchCodeCollection, patchHeap)
        );
    }

    const IWriteSectorsPatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzviWriteSdPatchCode(patchHeap);
        });
    }

    LoaderPlatformType GetPlatformType() const override { return LoaderPlatformType::Slot1; }

    bool HasDmaSdReads() const override { return true; }

private:
    const EzviApplySectorPatchCode* CreateApplySectorPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const
        {
            return patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new EzviApplySectorPatchCode(patchHeap);
            });
        }
};
