#pragma once
#include "../LoaderPlatform.h"
#include "DsxReadNandPatchCode.h"
#include "DsxReadNandDmaPatchCode.h"
#include "DsxWriteNandPatchCode.h"
#include "DsxSetupCommandPatchCode.h"
#include "DsxWaitBusyPatchCode.h"
#include "DsxZoneSwitchPatchCode.h"

/// @brief Implementation of LoaderPlatform for the DS-Xtreme flashcard
class DsxLoaderPlatform : public LoaderPlatform
{
public:
    const IReadSectorsPatchCode* CreateSdReadPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new DsxReadNandPatchCode(patchHeap,
                CreateSetupCommandPatchCode(patchCodeCollection, patchHeap),
                CreateWaitBusyPatchCode(patchCodeCollection, patchHeap),
                CreateZoneSwitchPatchCode(patchCodeCollection, patchHeap)
            );
        });
    }

    const IReadSectorsDmaPatchCode* CreateSdReadDmaPatchCode(PatchCodeCollection& patchCodeCollection,
        PatchHeap& patchHeap, const void* miiCardDmaCopy32Ptr) const override
    {
        return patchCodeCollection.AddUniquePatchCode<DsxReadNandDmaPatchCode>(
            patchHeap, miiCardDmaCopy32Ptr,
            CreateSetupCommandPatchCode(patchCodeCollection, patchHeap),
            CreateWaitBusyPatchCode(patchCodeCollection, patchHeap),
            CreateZoneSwitchPatchCode(patchCodeCollection, patchHeap)
        );
    }

    const IWriteSectorsPatchCode* CreateSdWritePatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const override
    {
        return patchCodeCollection.GetOrAddSharedPatchCode([&]
        {
            return new DsxWriteNandPatchCode(patchHeap,
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new DsxResetFpgaAddressPatchCode(patchHeap);
                }),
                CreateWaitBusyPatchCode(patchCodeCollection, patchHeap),
                CreateZoneSwitchPatchCode(patchCodeCollection, patchHeap),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new DsxWriteNandTransferBlockPatchCode(patchHeap,
                        CreateSetupCommandPatchCode(patchCodeCollection, patchHeap));
                }),
                patchCodeCollection.GetOrAddSharedPatchCode([&]
                {
                    return new DsxWriteNandStopTransmissionPatchCode(patchHeap,
                        CreateWaitBusyPatchCode(patchCodeCollection, patchHeap));
                })
            );
        });
    }

    LoaderPlatformType GetPlatformType() const override { return LoaderPlatformType::Slot1; }

    bool HasDmaSdReads() const override { return true; }

private:
    const DsxSetupCommandPatchCode* CreateSetupCommandPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const
        {
            return patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new DsxSetupCommandPatchCode(patchHeap);
            });
        }

    const DsxWaitBusyPatchCode* CreateWaitBusyPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const
        {
            return patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new DsxWaitBusyPatchCode(patchHeap);
            });
        }

    const DsxZoneSwitchPatchCode* CreateZoneSwitchPatchCode(
        PatchCodeCollection& patchCodeCollection, PatchHeap& patchHeap) const
        {
            return patchCodeCollection.GetOrAddSharedPatchCode([&]
            {
                return new DsxZoneSwitchPatchCode(patchHeap,
                    CreateSetupCommandPatchCode(patchCodeCollection, patchHeap)
                );
            });
        }
};
