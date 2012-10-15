@ write.S: write sycall wrapper
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/swi.h>
#include <bits/fileno.h>

	.file	"write.S"
	.text

	.global	write
write:
	stmfd sp!, {r4, r5}         @ store the variable register .
	                            @ so we don't clobber calling program.
	swi WRITE_SWI               @ call the write syscall.
	cmp r0, #0                  @ if 0 success.
	bpl no_error                @ branch on success.

	mov r4, r0                  @ store error num to our variable.
	mov r0, #-1                 @ set r0 to error value.
	EOR r4, r4, #0x80000000     @ negate error num.
	ldr r5, =errno              @ t
	str r4, [r5]                @ t

no_error:
	ldmfd sp!, {r4}             @ exit
	movs pc, lr
