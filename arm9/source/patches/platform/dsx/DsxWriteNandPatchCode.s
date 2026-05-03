.cpu arm7tdmi
.section "dsx_writenand", "ax"
.syntax unified
.thumb

// r0 = dst sector
// r1 = src
// r2 = sector count
.global dsx_writeNand
.type dsx_writeNand, %function
dsx_writeNand:
    push {r4-r7,lr}

    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

    // where actual user data is stored
    movs r3, #0x60
    lsls r3, r3, #8
    adds r0, r0, r3

    ldr r3, dsx_writeNand_waitBusy_address
    bl blx_r3

sector_loop:
    ldr r3, dsx_writeNand_zoneSwitch_address
    bl blx_r3
    ldr r3, dsx_writeNand_resetFpgaAddress_address
    bl blx_r3
    ldr r3, dsx_writeNand_resetFpgaAddress_address
    bl blx_r3
    ldr r3, dsx_writeNand_transferBlock_address
    bl blx_r3
    ldr r3, dsx_writeNand_waitBusy_address
    bl blx_r3

    adds r0, #1
    subs r2, #1
    bne sector_loop

    ldr r3, dsx_writeNand_stopTransmission_address

blx_r3:
    bx r3

.balign 4

.global dsx_writeNand_waitBusy_address
dsx_writeNand_waitBusy_address:
    .word 0

.global dsx_writeNand_resetFpgaAddress_address
dsx_writeNand_resetFpgaAddress_address:
    .word 0

.global dsx_writeNand_zoneSwitch_address
dsx_writeNand_zoneSwitch_address:
    .word 0

.global dsx_writeNand_transferBlock_address
dsx_writeNand_transferBlock_address:
    .word 0

.global dsx_writeNand_stopTransmission_address
dsx_writeNand_stopTransmission_address:
    .word 0

.pool

.section "dsx_writenand_transferblock", "ax"
.thumb
.global dsx_writeNand_transferBlock
.type dsx_writeNand_transferBlock, %function
dsx_writeNand_transferBlock:
    push {lr}

    movs r6, #1
    lsls r6, r6, #9

sector_data_loop:
    // write data to fpga buffer
    // for each u32 0xaabbccdd
    // 04 aa bb cc dd 00 00 00
    ldmia r1!, {r7}
    push {r0-r1}
    movs r0, #0x04
    movs r1, r7
    ldr r3, dsx_writeNand_setupCommand_address
    bl blx_r3_1
    pop {r0-r1}

    ldr r3, =0xA0586000
    str r3, [r4,#4]

sector_data_loop_wait_busy:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8 // check if transfer is done
    bcs sector_data_loop_wait_busy

    subs r6, r6, #4
    bne sector_data_loop

    push {r0-r1}
    movs r1, r0
    movs r0, #0x05
    ldr r3, dsx_writeNand_setupCommand_address
    bl blx_r3_1
    pop {r0-r1}

    movs r6, #0x41
    lsls r6, r6, #20

    ldr r3, =0xA7586FFF
    str r3, [r4,#4]

sector_commit_data_poll_transfer_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc sector_commit_data_poll_check_transfer_end
    ldr r5, [r6, #0x10]

sector_commit_data_poll_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8
    bcs sector_commit_data_poll_transfer_loop

    pop {pc}

blx_r3_1:
    bx r3

.balign 4

.global dsx_writeNand_setupCommand_address
dsx_writeNand_setupCommand_address:
    .word 0

.pool

.section "dsx_writenand_stoptransmission", "ax"
.thumb
.global dsx_writeNand_stopTransmission
.type dsx_writeNand_stopTransmission, %function
dsx_writeNand_stopTransmission:
    movs r3, #0xBC
    str r3, [r4, #0x8]
    movs r3, #0
    str r3, [r4, #0xC]

    movs r6, #0x41
    lsls r6, r6, #20

stopTransmission_poll_loop:
    ldr r3, =0xA7586FFF
    str r3, [r4,#4]

stopTransmission_poll_transfer_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc stopTransmission_poll_check_transfer_end
    ldr r5, [r6, #0x10]

stopTransmission_poll_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8
    bcs stopTransmission_poll_transfer_loop

    ldr r3, dsx_writeNand_stopTransmission_waitBusy_address
    bl blx_r3_2

    ldr r3, =191
    movs r4, #0x04
    lsls r4, r4, #24
wait_until_vcount_191:
    ldrh r7, [r4, #6]
    cmp r7, r3
    bne wait_until_vcount_191

wait_until_vcount_not_191:
    ldrh r7, [r4, #6]
    cmp r7, r3
    beq wait_until_vcount_191

wait_full_vcount:
    ldrh r7, [r4, #6]
    cmp r7, r3
    bne wait_full_vcount

    pop {r4-r7,pc}

blx_r3_2:
    bx r3

.balign 4

.global dsx_writeNand_stopTransmission_waitBusy_address
dsx_writeNand_stopTransmission_waitBusy_address:
    .word 0

.pool

.end
