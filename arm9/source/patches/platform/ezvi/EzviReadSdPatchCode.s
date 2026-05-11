.cpu arm7tdmi
.section "ezvi_readsd", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = dst
// r2 = sector count
.global ezvi_readSd
.type ezvi_readSd, %function
ezvi_readSd:
    push {r1,r4-r7,lr}

    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

sector_loop:
    movs r1, #0x51
    ldr r3, ezvi_readSd_applySector_address
    bl blx_r3

    movs r6, #0x40
    lsls r6, r6, #20 // 0x04100000

B9_wait_data_ready_loop:
    // Send CLK until SD data ready
    // B9 AA 01 00 00 00 00 00 
    movs r3, #1
    strh r3, [r4,#0xA]
    movs r3, #0
    str r3, [r4, #0xC]

    movs r3, #0xA7
    strb r3, [r4, #0x7]

B9_wait_data_ready_transfer_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc B9_wait_data_ready_check_transfer_end
    ldr r5, [r6, #0x10]

B9_wait_data_ready_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8
    bcs B9_wait_data_ready_transfer_loop

    lsls r5, r5, #24
    lsrs r5, r5, #24
    cmp r5, #0
    bne B9_wait_data_ready_loop

    // read data from SD
    // B9 AA 06 00 00 00 00 00
    movs r3, #0x6
    strh r3, [r4,#0xA]

    movs r3, #0xA1
    strb r3, [r4,#7]

    pop {r1}
transfer_data_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8 // check if data is ready
    bcc transfer_data_loop_check_transfer_end // if not skip reading

    ldr r3, [r6, #0x10]
    stmia r1!, {r3}

transfer_data_loop_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8 // check if transfer is done
    bcs transfer_data_loop

    adds r0, #1
    subs r2, #1
    bne sector_loop

    pop {r4-r7,pc}

blx_r3:
    bx r3

.balign 4

.global ezvi_readSd_applySector_address
ezvi_readSd_applySector_address:
    .word 0

.pool

.end