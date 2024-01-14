.cpu arm7tdmi
.section "scds_readsd", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = dst
// r2 = sector count
.global scds_readSd
.type scds_readSd, %function
scds_readSd:
    push {r4-r7,lr}

    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

SCDS_read_multiple_block:
    // request sd read for sector, sector << 9 == 0xaabbccdd
    // 33 00 52 aa bb cc dd 00

    // left shift sector address 9
    lsls r7, r0, #9

    ldr r3, scds_sendSdioCommand_rd_address
    push {r0-r2}
    movs r0, #18
    movs r1, r7
    movs r2, #1
    bl blx_r3
    pop {r0-r2}

    ldr r3, scds_readSectorLoop_address
    bl blx_r3

SCDS_stop_transmission:
    push {r0-r2}
    ldr r3, scds_sendSdioCommand_rd_address
    movs r0, #12
    movs r1, #0
    movs r2, #0
    bl blx_r3

    ldr r3, scds_sendCommand_rd_address
    movs r0, #1
    movs r5, #0x30
    str r5, [r4,#8]
    movs r5, #0x40
    strb r5, [r4,#9]
    bl blx_r3
    pop {r0-r2}

    pop {r4-r7,pc}

blx_r3:
    bx r3

.balign 4

.global scds_sendCommand_rd_address
scds_sendCommand_rd_address:
    .word 0

.global scds_sendSdioCommand_rd_address
scds_sendSdioCommand_rd_address:
    .word 0

.global scds_readSectorLoop_address
scds_readSectorLoop_address:
    .word 0

.pool

.end
