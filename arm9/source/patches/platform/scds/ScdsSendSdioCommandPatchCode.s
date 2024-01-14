.cpu arm7tdmi
.section "scds_sendsdiocommand", "ax"
.syntax unified
.thumb

.global scds_sendSdioCommand
.type scds_sendSdioCommand, %function
// r0: sdio cmd
// r1: sdio param
// r2: response type
scds_sendSdioCommand:
    push {r4,lr}

    // SDIO command
    // 33 00 AA BB BB BB BB CC
    // AA = SDIO cmd | 0x40
    // BBBBBBBB = parameter
    // CC = response type

    // start copy to MCCMD1
    movs r3, #0x33
    str r3, [r4,#0x8]
    movs r3, #0x40
    ands r3, r3, r0
    strb r3, [r4,#0xA]
    lsrs r3, r1, #24
    strb r3, [r4,#0xB]
    lsrs r3, r1, #16
    strb r3, [r4,#0xC]
    lsrs r3, r1, #8
    strb r3, [r4,#0xD]
    strb r1, [r4,#0xE]
    strb r2, [r4,#0xF]

    ldr r3, scds_sendCommand_sdio_address
    movs r0, #1
    bl blx_r3

    pop {r4,pc}

blx_r3:
    bx r3

.balign 4

.global scds_sendCommand_sdio_address
scds_sendCommand_sdio_address:
    .word 0

.pool

.end
