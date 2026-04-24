.cpu arm7tdmi
.section "scds_readsectorloop", "ax"
.syntax unified
.thumb

// r0 = sector, unused
// r1 = destination
// r2 = sector count
.global scds_readSectorLoop
.type scds_readSectorLoop, %function
scds_readSectorLoop:
    push {r0,r4-r5,lr}
    ldr r5, =0x04100010

sector_loop:
    movs r3, #0x34
    str r3, [r4,#8]
    ldr r3, scds_sendCommand_rdsect_address
    movs r0, #1
    bl blx_r3

SCDS_read_block:
    movs r3, #0x36
    str r3, [r4,#8]
    movs r3, #0xA1
    strb r3, [r4,#7]

SCDS_read_block_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc SCDS_read_block_loop_check_transfer_end

    ldr r3, [r5]
    stmia r1!, {r3}

SCDS_read_block_loop_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8 // check if transfer is done
    bcs SCDS_read_block_loop

    subs r2, #1
    bne sector_loop

    pop {r0,r4-r5,pc}

blx_r3:
    bx r3

.balign 4

.global scds_sendCommand_rdsect_address
scds_sendCommand_rdsect_address:
    .word 0

.pool

.end
