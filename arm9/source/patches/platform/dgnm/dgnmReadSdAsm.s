.cpu arm7tdmi
.section "dgnm_readsd", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = dst
// r2 = sector count
.global dgnm_readSd
.type dgnm_readSd, %function
dgnm_readSd:
    push {r4-r7,lr}

    movs r7, #1
    lsls r7, r7, #9

    ldr r4, =0x040001A0

sector_loop:
    movs r5, #17
    ldr r3, dgnm_readSd_sendSdioCommand_address
    bl blx_r3

SDIO_wait_data_ready:
    bl SPI_null_cmd
    cmp r5, #0xFE
    bne SDIO_wait_data_ready

    movs r6, r7 // counter

SPI_read_sector_byte:
    bl SPI_null_cmd
    strb r5, [r1]

    adds r1, #1
    subs r6, #1
    bne SPI_read_sector_byte

    // Send fake CRC
    bl SPI_null_cmd
    bl SPI_null_cmd

    adds r0, #1
    subs r2, #1
    bne sector_loop

    pop {r4-r7,pc}

SPI_null_cmd:
    movs r5, #0xFF
SPI_write_cmd:
#include "dgnmSendSpiCommand.s.in"

blx_r3:
    bx r3

.balign 4

.global dgnm_readSd_sendSdioCommand_address
dgnm_readSd_sendSdioCommand_address:
    .word 0

.pool

.end
