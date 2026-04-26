.cpu arm7tdmi
.section "dsx_readnand", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = dst
// r2 = sector count
.global dsx_readNand
.type dsx_readNand, %function
dsx_readNand:
    push {r4-r7,lr}

    // where actual user data is stored
    movs r7, #0x60
    lsls r7, r7, #8
    adds r0, r0, r7

    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

sector_loop:
    ldr r3, dsx_readNand_waitBusy_address
    bl blx_r3
    ldr r3, dsx_readNand_zoneSwitch_address
    bl blx_r3

    push {r0-r1}
    movs r1, r0
    movs r0, #0xBF
    ldr r3, dsx_readNand_setupCommand_address
    bl blx_r3
    pop {r0-r1}

    ldr r3, =0xA15868F8
    str r3, [r4,#4]

    ldr r6, =0x04100010

BF_data_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8 // check if data is ready
    bcc BF_data_loop_check_transfer_end // if not skip reading
    
    ldr r3, [r6]
    stmia r1!, {r3}

BF_data_loop_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8 // check if transfer is done
    bcs BF_data_loop

    adds r0, #1
    subs r2, #1
    bne sector_loop

    pop {r4-r7,pc}

blx_r3:
    bx r3

.balign 4

.global dsx_readNand_waitBusy_address
dsx_readNand_waitBusy_address:
    .word 0

.global dsx_readNand_zoneSwitch_address
dsx_readNand_zoneSwitch_address:
    .word 0

.global dsx_readNand_setupCommand_address
dsx_readNand_setupCommand_address:
    .word 0

.pool

.end
