@ read.S: read syscall wrapper
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/swi.h>
#include <bits/fileno.h>
	.file	"read.S"
	.text

	.global	read
read:
	stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi READ_SWI                  @ call the read syscall. (note: uses OABI
                                @ calling conventions)
	cmp r0, #0                    @ return 0 if no error
	bpl no_error                  @ skip code if no error

  @ handle error
	mov r4, r0                    @ store error num returned to our variable.
	mov r0, #-1                   @ set return to -1
	EOR r4, r4, #0x80000000       @ flip sign of err num from syscall to positive
	ldr r5, =errno                @ get the address of the global variable errno
	str r4, [r5]                  @ set errno to positive of syscall return

no_error:
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
	@movs pc, lr                  @ return to the calling function
