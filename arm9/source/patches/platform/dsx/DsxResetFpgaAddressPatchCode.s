.cpu arm7tdmi
.section "dsx_resetfpgaaddress", "ax"
.syntax unified
.thumb

.global dsx_resetFpgaAddress
.type dsx_resetFpgaAddress, %function
dsx_resetFpgaAddress:
    push {r0-r1,r4-r6,lr}

    ldr r4, =0x040001A0
    movs r6, #0x80
    strb r6, [r4,#1]

    movs r5, #0x03
    str r5, [r4, #0x8]
    movs r5, #0
    str r5, [r4, #0xC]

dsx_resetFpgaAddress_poll_loop:
    ldr r6, =0xA0586000
    str r6, [r4,#4]

dsx_resetFpgaAddress_poll_check_transfer_end:
    ldrb r6, [r4,#7]
    lsrs r6, r6, #8
    bcs dsx_resetFpgaAddress_poll_check_transfer_end

    pop {r0-r1,r4-r6,pc}

.balign 4

.pool

.end
