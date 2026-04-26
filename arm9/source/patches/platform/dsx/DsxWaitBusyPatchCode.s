.cpu arm7tdmi
.section "dsx_waitbusy", "ax"
.syntax unified
.thumb

.global dsx_waitBusy
.type dsx_waitBusy, %function
dsx_waitBusy:
    push {r4-r7,lr}

    ldr r4,=0x040001A0
    movs r7, #0x80
    strb r7, [r4,#1]

    // 02 00 00 00 00 00 00 00
    movs r7, #0x02
    str r7, [r4,#0x8]
    movs r7, #0
    str r7, [r4,#0xC]

    ldr r6, =0x04100010

dsx_waitBusy_poll_loop:
    ldr r7, =0xA7586800
    str r7, [r4,#4]

dsx_waitBusy_poll_transfer_loop:
    ldrb r7, [r4,#6]
    lsrs r7, r7, #8
    bcc dsx_waitBusy_poll_check_transfer_end
    ldr r7, [r6]

dsx_waitBusy_poll_check_transfer_end:
    ldrb r5, [r4,#7]
    lsrs r5, r5, #8
    bcs dsx_waitBusy_poll_transfer_loop

    cmp r7, #0
    bne dsx_waitBusy_poll_loop

    pop {r4-r7,pc}

.balign 4

.pool

.end
