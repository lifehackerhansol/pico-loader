.cpu arm946e-s
.section "r4_readromdma", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = previous src sector
// r2 = dma channel
// r3 = dst
.global r4_readRomDma
.type r4_readRomDma, %function
r4_readRomDma:
    push {r4-r7,lr}
    lsls r0, r0, #9 // rom sector to rom byte address
    ldr r4, =0x040001A0
    movs r3, #0x80
    strb r3, [r4,#1]

    // request rom read at rom address 0xaabbccdd
    // B6 aa bb cc dd 00 00 00
    movs r3, #0xB6
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

    ldr r1, =0x04100010

B6_poll_loop:
    ldr r3, =0xA7586000
    str r3, [r4,#4]

B6_poll_transfer_loop:
    ldrb r3, [r4,#6]
    lsrs r3, r3, #8
    bcc B6_poll_check_transfer_end
    ldr r2, [r1]

B6_poll_check_transfer_end:
    ldrb r3, [r4,#7]
    lsrs r3, r3, #8
    bcs B6_poll_transfer_loop

    cmp r2, #0
    bne B6_poll_loop

    // Setup data read card command
    movs r7, #0xB7
    str r7, [r4,#0x8]
    str r5, [r4,#0xC] // r5 is 0 here

readDataWithDma:
    movs r0, r2 // DMA channel
    movs r2, r3 // Destination
    movs r3, #1
    lsls r3, r3, #9 // (1 << 9) = 512 = count

    ldr r6, r4_readRomDma_miiCardDmaCopy32Ptr
    blx r6

B7_data_dma:
    movs r7, #0xC0 // select rom mode, with irq
    strb r7, [r4,#0x1]
    ldr r7, =0xA1586000
    str r7, [r4,#0x4]

    pop {r4-r7,pc}

.balign 4

.global r4_readRomDma_miiCardDmaCopy32Ptr
r4_readRomDma_miiCardDmaCopy32Ptr:
    .word 0

.pool

.global r4_finishReadRomDma
.type r4_finishReadRomDma, %function
r4_finishReadRomDma:
    // No cleanup needed on this platform.
    bx lr

.balign 4

.pool

.end
