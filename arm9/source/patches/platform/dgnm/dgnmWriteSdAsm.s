.cpu arm7tdmi
.section "dgnm_writesd", "ax"
.syntax unified
.thumb

// r0 = dst sector
// r1 = src
// r2 = sector count
.global dgnm_writeSd
.type dgnm_writeSd, %function
dgnm_writeSd:
    push {r4-r7,lr}

    movs r7, #1
    lsls r7, r7, #9

    ldr r4, =0x040001A0

// r5 = REG_MCD0 data in/out
sector_loop:
    // send CMD24
    movs r5, #24
    ldr r3, dgnm_writeSd_sendSdioCommand_address
    bl blx_r3

    movs r6, r7 // counter

    // Start data transfer!
    movs r5, #0xFE
    bl SPI_write_cmd

    // current write offset
    movs r6, r7

SPI_write_sector_byte:
    ldrb r5, [r1]
    bl SPI_write_cmd

    adds r1, #1
    subs r6, #1
    bne SPI_write_sector_byte

    // Send fake CRC
    bl SPI_null_cmd
    bl SPI_null_cmd

// This is supposed to have a timeout
// But we don't have a handler for failed reads, so just loop forever
// we should also be checking for response of 0x5 before checking 0
// so let's scrap that too
SPI_write_sector_check_transfer_end:
    bl SPI_null_cmd
    cmp r5, #0
    beq SPI_write_sector_check_transfer_end

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

.global dgnm_writeSd_sendSdioCommand_address
dgnm_writeSd_sendSdioCommand_address:
    .word 0

.pool

.end
