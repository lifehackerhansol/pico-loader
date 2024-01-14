.cpu arm7tdmi
.section "scds_sendcommand", "ax"
.syntax unified
.thumb

.global scds_sendCommand
.type scds_sendCommand, %function
// r0: function to jump to
// r0=0: SCDS_send_command
// r0=1: SCDS_send_command and SCDS_wait_busy
scds_sendCommand:
    push {r4-r6,lr}

SCDS_send_command:
    ldr r4, =0x040001A0
    ldr r5, =0xA7180000
    ldr r6, =0x04100010
    str r5, [r4,#4]

SCDS_send_command_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc SCDS_send_command_loop

    ldr r3, [r6] // flush buffer

    cmp r0, #0
    beq SCDS_return

SCDS_wait_busy:
    /*
        Command to check if cartridge is busy with an operation
        The rest of the command buffer is ignored
        38 00 00 00 00 00 00 00
    */
    movs r3, #0x38
    str r3, [r4,#8]
    str r5, [r4,#4]

// should return 0 when idle
SCDS_wait_busy_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc SCDS_wait_busy_loop

    ldr r3, [r6]
    cmp r3, #0
    bne SCDS_wait_busy

SCDS_return:
    pop {r4-r6,pc}
