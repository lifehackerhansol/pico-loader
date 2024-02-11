#include "common.h"
#include "DGNMLoaderPlatform.h"

#define GMTF_CMD_SPI_ENABLE    0xF200000000CC0000ull

void DGNMLoaderPlatform::PrepareRomBoot(
    u32 romDirSector, u32 romDirSectorOffset) const
{
    card_romSetCmd(GMTF_CMD_SPI_ENABLE);
    card_romStartXfer(0x20586000, false);
    card_romWaitBusy();

    REG_MCCNT0 = MCCNT0_ENABLE | MCCNT0_MODE_SPI | MCCNT0_SPI_HOLD_CS;
}
