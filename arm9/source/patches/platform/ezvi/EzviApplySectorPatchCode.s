.cpu arm7tdmi
.section "ezvi_applysector", "ax"
.syntax unified
.thumb

// r0: sector address
// r1: cmd | 0x40
// r4: register base 0x040001A0
// r5-r7 will also be trashed
.global ezvi_applySector
.type ezvi_applySector, %function
ezvi_applySector:

.global ezvi_applySector_sdsc_shift
ezvi_applySector_sdsc_shift:
    lsls r5, r0, #9

    // SDIO CMD17 request for sector 0xaabbccdd
    // B9 AA 00 51 aa bb cc dd
    movs r7, #0xB9
    str r7, [r4,#0x8]
    movs r7, #0xAA
    strb r7, [r4,#0x9]
    strb r1, [r4,#0xB]
    lsrs r7, r5, #24
    strb r7, [r4,#0xC]
    lsrs r7, r5, #16
    strb r7, [r4,#0xD]
    lsrs r7, r5, #8
    strb r7, [r4,#0xE]
    strb r5, [r4,#0xF]

    movs r5, #0x40
    lsls r5, r5, #20 // 0x041000000

B9_poll_loop:
    ldr r7, =0xA7586000
    str r7, [r4,#0x4]

B9_poll_transfer_loop:
    ldrb r7, [r4,#0x6]
    lsrs r7, r7, #8
    bcc B9_poll_check_transfer_end
    ldr r5, [r1, #0x10]

B9_poll_check_transfer_end:
    ldrb r7, [r4,#0x7]
    lsrs r7, r7, #8
    bcs B9_poll_transfer_loop

    bx lr

.balign 4

.pool

.end
