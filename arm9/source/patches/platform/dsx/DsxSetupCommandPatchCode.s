.cpu arm7tdmi
.section "dsx_setupcommand", "ax"
.syntax unified
.thumb

// r0 = cmd
// r1 = param
.global dsx_setupCommand
.type dsx_setupCommand, %function
dsx_setupCommand:
    push {r4-r5,lr}
    ldr r4, =0x040001A0

    // cmd - byte 0
    // param - byte 1-4
    strb r0, [r4,#0x8]
    lsrs r5, r1, #24
    strb r5, [r4,#0x9]
    lsrs r5, r1, #16
    strb r5, [r4,#0xA]
    lsrs r5, r1, #8
    strb r5, [r4,#0xB]
    lsls r5, r1, #24
    lsrs r5, r5, #24 // r5 = dd
    str r5, [r4,#0xC] // storing as little-endian puts the bottom 8 bits as first byte

    pop {r4-r5,pc}

.balign 4

.pool

.end
