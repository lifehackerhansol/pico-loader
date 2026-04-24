.cpu arm946e-s
.section "scds_readsddma", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = previous src sector
// r2 = dma channel
// r3 = dst
.global scds_readSdDma
.type scds_readSdDma, %function
scds_readSdDma:
    push {r4-r7,lr}

    ldr r4, =0x040001A0
    movs r7, #0x80
    strb r7, [r4,#1]

    // left shift sector address 9
    lsls r7, r0, #9

    cmp r1, #0 // if first sector
    beq SCDS_read_multiple_block_dma // first sector, skip poll

    subs r1, r0, r1
    cmp r1, #1 // if sequential
    beq SCDS_read_multiple_block_dma_request_block // skip CMD18

    bl scds_finishReadSdDma

SCDS_read_multiple_block_dma:
    // request sd read for sector, sector << 9 == 0xaabbccdd
    // 33 00 52 aa bb cc dd 00

    push {r0-r2}
    movs r0, #18
    movs r1, r7
    movs r2, #1
    ldr r7, scds_sendSdioCommand_dma_address
    blx r7
    pop {r0-r2}

SCDS_read_multiple_block_dma_request_block:
    movs r7, #0x34
    str r7, [r4,#8]
    movs r0, #1
    ldr r7, scds_sendCommand_dma_address
    blx r7

    movs r0, r2 // DMA channel
    ldr r1, =0x04100010
    movs r2, r3 // Destination
    movs r3, #1
    lsls r3, r3, #9 // (1 << 9) = 512 = count

    ldr r7, scds_miiCardDmaCopy32Ptr
    blx r7

SCDS_dma_start_transfer:
    movs r7, #0xC0 // select rom mode, with irq
    strb r7, [r4,#0x1]
    movs r7, #0x36
    strb r7, [r4,#0x8]
    movs r7, #0xA1
    strb r7, [r4,#0x7]
    pop {r4-r7,pc}

.global scds_finishReadSdDma
.type scds_finishReadSdDma, %function
scds_finishReadSdDma:
    ldr r0, scds_sdStopTransmission_dma_address
    bx r0

.balign 4

.global scds_sendCommand_dma_address
scds_sendCommand_dma_address:
    .word 0

.global scds_sendSdioCommand_dma_address
scds_sendSdioCommand_dma_address:
    .word 0

.global scds_sdStopTransmission_dma_address
scds_sdStopTransmission_dma_address:
    .word 0

.global scds_miiCardDmaCopy32Ptr
scds_miiCardDmaCopy32Ptr:
    .word 0

.pool

.end
