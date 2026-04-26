.cpu arm7tdmi
.section "dsx_zoneswitch", "ax"
.syntax unified
.thumb

// r0 = src sector
.global dsx_zoneSwitch
.type dsx_zoneSwitch, %function
dsx_zoneSwitch:
    push {r0-r1,r3-r7,lr}

    // check if we are switching zones
    // zone = (sector >> 8 / 1000)
    lsrs r7, r0, #8

    ldr r6, =dsx_currentZone
    cmp r7, r6
    beq skip_zone_switch

    adr r4, dsx_divide10
    bl blx_r4
    adr r6, dsx_currentZone
    str r7, [r6]

    movs r1, r0
    movs r0, #0xBF
    ldr r4, dsx_zoneSwitch_setupCommand_address
    bl blx_r4
    ldr r4, =0x040001A0
    ldr r6, =0x04100010

BF_poll_loop:
    ldr r7, =0xA1586FFF
    str r7, [r4,#4]

BF_poll_transfer_loop:
    ldrb r7, [r4,#6]
    lsrs r7, r7, #8
    bcc BF_poll_check_transfer_end
    ldr r7, [r6]

BF_poll_check_transfer_end:
    ldrb r7, [r4,#7]
    lsrs r7, r7, #8
    bcs BF_poll_transfer_loop

skip_zone_switch:
    pop {r0-r1,r3-r7,pc}

blx_r4:
    bx r4

.balign 4

dsx_currentZone:
    .word 0xFFFFFFFF

.global dsx_zoneSwitch_setupCommand_address
dsx_zoneSwitch_setupCommand_address:
    .word 0

.pool

.arm
dsx_divide10:
    ldr     r6, =0xCCCCCCCD
    umull   r4, r5, r7, r6
    lsrs    r7, r5, #3
    bx lr

.balign 4

.pool

.end
