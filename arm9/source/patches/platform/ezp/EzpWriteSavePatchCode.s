.cpu arm7tdmi
.section "ezp_writesave", "ax"
.syntax unified
.thumb

// r0 = dst sector
// r1 = src
.global ezp_writeSave
.type ezp_writeSave, %function
ezp_writeSave:
    push {r4-r7,lr}
    lsls r0, r0, #9 // save sector to save byte address
    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

    // write at save address 0xaabbccdd
    // BD aa bb cc dd 00 00 00
    movs r3, #0xBD
    strb r3, [r4,#0x8]
    lsrs r3, r0, #24
    strb r3, [r4,#0x9]
    lsrs r3, r0, #16
    strb r3, [r4,#0xA]
    lsrs r3, r0, #8
    strb r3, [r4,#0xB]
    lsls r0, r0, #24
    lsrs r7, r0, #24 // r7 = dd
    str r7, [r4,#0xC] // storing as little-endian puts the bottom 8 bits as first byte

    ldr r6, =0x04100010

    ldr r3, =0xE1586000
    str r3, [r4,#4]

BD_data_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8 // check if ready to write
    bcc BD_data_loop_check_transfer_end // if not skip reading
    
    ldmia r1!, {r3}
    str r3, [r6]

BD_data_loop_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8 // check if transfer is done
    bcs BD_data_loop

    movs r3, #0xBE
    strb r3, [r4,#0x8]

BE_poll_loop:
    ldr r3, =0xA7586000
    str r3, [r4,#4]

BE_poll_transfer_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc BE_poll_check_transfer_end
    ldr r2, [r6]

BE_poll_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8
    bcs BE_poll_transfer_loop

    cmp r2, #0
    bne BE_poll_loop

    pop {r4-r7,pc}

.balign 4

.pool

.end
