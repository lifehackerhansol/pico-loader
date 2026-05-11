.cpu arm946e-s
.section "ezvi_readsddma", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = previous src sector
// r2 = dma channel
// r3 = dst
.global ezvi_readSdDma
.type ezvi_readSdDma, %function
ezvi_readSdDma:
    push {r4-r7,lr}

    ldr r4, =0x040001A0
    movs r7, #0x80
    strb r7, [r4,#0x1]

    movs r1, #0x51
    ldr r7, ezvi_readSdDma_applySector_address
    blx r7

B9_wait_data_ready_loop:
    // Send CLK until SD data ready
    // B9 AA 01 00 00 00 00 00 
    movs r7, #1
    strh r7, [r4,#0xA]
    movs r7, #0
    str r7, [r4, #0xC]

    movs r7, #0xA7
    strb r7, [r4, #0x7]

    ldr r1, =0x04100010

B9_wait_data_ready_transfer_loop:
    ldrb r7, [r4,#6]
    lsrs r7, r7, #8
    bcc B9_wait_data_ready_check_transfer_end
    ldr r5, [r6]

B9_wait_data_ready_check_transfer_end:
    ldrb r7, [r4,#7]
    lsrs r7, r7, #8
    bcs B9_wait_data_ready_transfer_loop

    lsls r5, r5, #24
    lsrs r5, r5, #24
    cmp r5, #0
    bne B9_wait_data_ready_loop

    // Setup data read card command
    // B9 AA 06 00 00 00 00 00
    movs r7, #0x6
    strh r7, [r4,#0xA]

readDataWithDma:
    movs r0, r2 // DMA channel
    movs r2, r3 // Destination
    movs r3, #1
    lsls r3, r3, #9 // (1 << 9) = 512 = count

    ldr r6, ezvi_readSdDma_miiCardDmaCopy32Ptr
    blx r6

    movs r7, #0xC0 // select rom mode, with irq
    strb r7, [r4,#0x1]
    movs r7, #0xA1
    strb r7, [r4,#7]

    pop {r4-r7,pc}

.balign 4

.global ezvi_readSdDma_miiCardDmaCopy32Ptr
ezvi_readSdDma_miiCardDmaCopy32Ptr:
    .word 0

.global ezvi_readSdDma_applySector_address
ezvi_readSdDma_applySector_address:
    .word 0

.pool

.global ezvi_finishReadSdDma
.type ezvi_finishReadSdDma, %function
ezvi_finishReadSdDma:
    // No cleanup needed on this platform.
    bx lr

.balign 4

.pool

.end
