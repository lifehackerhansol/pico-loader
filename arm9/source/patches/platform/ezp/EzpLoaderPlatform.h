#pragma once
#include "../LoaderPlatform.h"
#include "EzpReadRomPatchCode.h"
#include "EzpReadSectorsPatchCode.h"
#include "EzpReadSectorsDmaPatchCode.h"
#include "EzpReadSdDataPatchCode.h"
#include "EzpReadSavePatchCode.h"
#include "EzpWriteSectorsPatchCode.h"
#include "EzpWriteSavePatchCode.h"

/// @brief Implementation of LoaderPlatform for the EZ-Flash Parallel flashcard
class EzpLoaderPlatform : public LoaderPlatform
{
public:
    const IReadSectorsPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzpReadSectorsPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new EzpReadSdDataPatchCode(patchHeap);
                }));
        });
    }

    const IReadSectorsDmaPatchCode* CreateSdReadDmaPatchCode(PatchCodeCollection& patchCodeCollection,
        PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr) const override
    {
        return patchCodeCollection.AddUniquePatchCode<EzpReadSectorsDmaPatchCode>(
            patchHeap, miiCardDmaCopy32Ptr);
    }

    const IWriteSectorsPatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzpWriteSectorsPatchCode(patchHeap);
        });
    }

    const IReadSectorsPatchCode* CreateRomReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzpReadRomPatchCode(patchHeap);
        });
    }

    const IReadSectorsPatchCode* CreateSaveReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzpReadSavePatchCode(patchHeap);
        });
    }

    const IWriteSectorsPatchCode* CreateSaveWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new EzpWriteSavePatchCode(patchHeap);
        });
    }

    // bool HasDmaSdReads() const override { return true; }

    bool HasRomReads() const override { return true; }

    bool HasSaveReadWrite() const override { return true ;}

    void PrepareRomBoot(u32 romDirSector, u32 romDirSectorOffset, u32 saveDirSector, u32 saveDirSectorOffset) const override;

    LoaderPlatformType GetPlatformType() const override { return LoaderPlatformType::Slot1; }

private:
    u32 ReadCardInfo(void) const;
};
