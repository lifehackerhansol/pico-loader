.cpu arm7tdmi
.section "scds_writesingleblock", "ax"
.syntax unified
.thumb

// r0 = dst sector
// r1 = src
// r2 = sector count
.global scds_writeSingleBlock
.type scds_writeSingleBlock, %function
scds_writeSingleBlock:

    push {r4-r5,lr}

    // Set up a counter for number of bytes transferred
    // This will be incremented and checked in further labels
    movs r5, #0

SCDS_write_512:
    /*
        set up command for data transfer
        aaaa = current number of bytes sent
        bbbbbbbb = u32 from buffer to write, bswap'd
        37 aa aa bb bb bb bb 00
    */

    movs r3, #0x37
    str r3, [r4,#0x8]
    // u16 of number of bytes sent
    lsrs r3, r5, #8
    strb r3, [r4,#0x9]
    strb r5, [r4,#0xA]
    // u32 data from buffer, bswap32'd
    ldmia r1!, {r3}
    strb r3, [r4,#0xE]
    lsrs r3, r3, #8
    strb r3, [r4,#0xD]
    lsrs r3, r3, #8
    strb r3, [r4,#0xC]
    lsrs r3, r3, #8
    strb r3, [r4,#0xB]

    movs r0, #0
    bl blx_r7

SCDS_write_512_loop:
    adds r5, #4
    lsrs r3, r5, #2
    cmp r3, #128 // 128 words in 512 bytes
    blt SCDS_write_512

    pop {r4-r5,pc}

blx_r7:
    bx r7

.balign 4

.global scds_sendCommand_wrsb_address
scds_sendCommand_wrsb_address:
    .word 0

.pool

.end
