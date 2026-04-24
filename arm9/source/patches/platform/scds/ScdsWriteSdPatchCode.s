.cpu arm7tdmi
.section "scds_writesd", "ax"
.syntax unified
.thumb

// r0 = dst sector
// r1 = src
// r2 = sector count
.global scds_writeSd
.type scds_writeSd, %function
scds_writeSd:
    push {r4-r7,lr}

    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

sector_loop:
    // write at sd sector, sector << 9 == 0xaabbccdd
    // 33 00 58 AA BB CC DD 01

    // left shift sector address 9
    lsls r7, r0, #9

    ldr r3, scds_sendSdioCommand_wr_address
    push {r0-r2}
    movs r0, #24
    movs r1, r7
    movs r2, #1
    bl blx_r3

    ldr r3, scds_sendCommand_wr_address
    movs r0, #1
    movs r6, #0x30
    str r6, [r4,#8]
    movs r6, #0x40
    strb r6, [r4,#9]
    bl blx_r3

    ldr r3, scds_writeSingleBlock_address
    bl blx_r3

SCDS_stop_transmission:
    /*
        Stop transmission of data
        The rest of the command buffer is ignored
        35 00 00 00 00 00 00 00
    */
    ldr r3, scds_sendCommand_wr_address
    movs r6, #0x35
    str r6, [r4,#0x8]
    movs r0, #1
    bl blx_r3

    pop {r0-r2}

    adds r0, #1
    subs r2, #1
    bne sector_loop

    pop {r4-r7,pc}

blx_r3:
    bx r3

.balign 4

.global scds_sendCommand_wr_address
scds_sendCommand_wr_address:
    .word 0

.global scds_sendSdioCommand_wr_address
scds_sendSdioCommand_wr_address:
    .word 0

.global scds_writeSingleBlock_address
scds_writeSingleBlock_address:
    .word 0


.pool

.end
