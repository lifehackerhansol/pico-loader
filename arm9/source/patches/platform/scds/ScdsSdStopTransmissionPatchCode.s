.cpu arm7tdmi
.section "scds_sdstoptransmission", "ax"
.syntax unified
.thumb

.global scds_sdStopTransmission
.type scds_sdStopTransmission, %function
scds_sdStopTransmission:
    push {r0-r4,lr}
    ldr r4, =0x040001A0

    ldr r3, scds_sendSdioCommand_cmd12_address
    movs r0, #12
    movs r1, #0
    movs r2, #0
    bl blx_r3

    ldr r3, scds_sendCommand_cmd12_address
    movs r0, #1
    movs r1, #0x30
    str r1, [r4,#8]
    movs r1, #0x40
    strb r1, [r4,#9]
    bl blx_r3

    pop {r0-r4,pc}

blx_r3:
    bx r3

.balign 4

.global scds_sendCommand_cmd12_address
scds_sendCommand_cmd12_address:
    .word 0

.global scds_sendSdioCommand_cmd12_address
scds_sendSdioCommand_cmd12_address:
    .word 0

.pool

.end
