.cpu arm7tdmi
.section "dgnm_sendspicommand", "ax"
.syntax unified
.thumb


// r4 should be 0x040001A0 here
// r5 = REG_MCD0 data in/out
.global dgnm_sendSpiCommand
.type dgnm_sendSpiCommand, %function
#include "dgnmSendSpiCommand.s.in"

.balign 4

.pool

.end
