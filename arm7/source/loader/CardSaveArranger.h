#pragma once
#include "ndsHeader.h"

/// @brief Struct holding the result of setting up card save data.
struct CardSaveResult
{
    /// @brief \c true when the rom uses a save file, or \c false otherwise.
    bool hasSave;

    /// @brief The SD sector of the save directory.
    u32 saveDirSector;

    /// @brief The offset of the save file entry in the save directory sector.
    u32 saveDirSectorOffset;
};

/// @brief Class for setting up the save file for retail card roms.
class CardSaveArranger
{
public:
    /// @brief Sets up the save file at \p savePath for a retail card rom with the given \p gameCode.
    /// @param header The header of the retail card rom.
    /// @param savePath The desired save file path.
    /// @param result Struct in which extra information about the save file is returned.
    /// @return \c true when setting up the save was successful, or \c false otherwise.
    bool SetupCardSave(const nds_header_ntr_t* header, const TCHAR* savePath, CardSaveResult& result) const;
};
