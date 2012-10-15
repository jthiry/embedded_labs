@ read.S: read sycall wrapper
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date

#include <bits/swi.h>
#include <bits/fileno.h>
	.file	"read.S"
	.text

	.global	read
read:
	stmfd sp!, {r4}
	swi READ_SWI
	cmp r0, #0
	bpl no_error

	mov r4, r0
	mov r0, #-1
	EOR r4, r4, #0x80000000 
	ldr r5, =errno
	str r4, [r5]

no_error:
	ldmfd sp!, {r4}
	movs pc, lr


