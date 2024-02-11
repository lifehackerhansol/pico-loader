.cpu arm7tdmi
.section "dgnm_sendsdiocommand", "ax"
.syntax unified
.thumb


// r5 = SDIO command
// r6 = sector
.global dgnm_sendSdioCommand
.type dgnm_sendSdioCommand, %function
dgnm_sendSdioCommand:
    bl SPI_write_cmd
    // left shift sector address 9
    lsls r7, r0, #9
    // start delivering sector address
    lsrs r5, r7, #24
    bl SPI_write_cmd
    lsrs r5, r7, #16
    bl SPI_write_cmd
    lsrs r5, r7, #8
    bl SPI_write_cmd
    movs r5, r7
    bl SPI_write_cmd
    movs r5, #0x95 // fake CRC
    bl SPI_write_cmd

// This is supposed to respond with 0, and there should be a timeout period
// But we don't have error handling in-game, so ignore all that
SDIO_send_cmd_wait_busy:
    bl SPI_null_cmd
    cmp r5, #0xFF
    beq SDIO_send_cmd_wait_busy

    bx lr

SPI_null_cmd:
    movs r5, #0xFF
SPI_write_cmd:
#include "dgnmSendSpiCommand.s.in"

.balign 4

.pool

.end
