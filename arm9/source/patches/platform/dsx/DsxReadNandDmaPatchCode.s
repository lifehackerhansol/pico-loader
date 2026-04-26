.cpu arm946e-s
.section "dsx_readnanddma", "ax"
.syntax unified
.thumb

// r0 = src sector
// r1 = previous src sector
// r2 = dma channel
// r3 = dst
.global dsx_readNandDma
.type dsx_readNandDma, %function
dsx_readNandDma:
    push {r4-r7,lr}

    // where actual user data is stored
    movs r7, #0x60
    lsls r7, r7, #8
    adds r0, r0, r7

    ldr r4, =0x040001A0
    movs r7, #0x80
    strb r7, [r4,#0x1]

    ldr r7, dsx_readNandDma_waitBusy_address
    bl blx_r7
    ldr r7, dsx_readNandDma_zoneSwitch_address
    bl blx_r7

    // Setup data read card command
    movs r1, r0
    movs r0, #0xBF
    ldr r7, dsx_readNandDma_setupCommand_address
    bl blx_r7

    ldr r1, =0x04100010

    movs r0, r2 // DMA channel
    movs r2, r3 // Destination
    movs r3, #1
    lsls r3, r3, #9 // (1 << 9) = 512 = count

    ldr r6, dsx_readNandDma_miiCardDmaCopy32Ptr
    blx r6

    movs r7, #0xC0 // select rom mode, with irq
    strb r7, [r4,#0x1]
    ldr r7, =0xA15868F8
    str r7, [r4,#0x4]

    pop {r4-r7,pc}

blx_r7:
    bx r7

.balign 4

.global dsx_readNandDma_miiCardDmaCopy32Ptr
dsx_readNandDma_miiCardDmaCopy32Ptr:
    .word 0

.global dsx_readNandDma_waitBusy_address
dsx_readNandDma_waitBusy_address:
    .word 0

.global dsx_readNandDma_zoneSwitch_address
dsx_readNandDma_zoneSwitch_address:
    .word 0

.global dsx_readNandDma_setupCommand_address
dsx_readNandDma_setupCommand_address:
    .word 0

.pool

.global dsx_finishReadNandDma
.type dsx_finishReadNandDma, %function
dsx_finishReadNandDma:
    // No cleanup needed on this platform.
    bx lr

.balign 4

.pool

.end
