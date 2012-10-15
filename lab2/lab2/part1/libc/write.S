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
	stmfd sp!, {r4, r5}         @ store r4, r5 on the stack so that we can use them in our wrapper
	                            @ 
	swi WRITE_SWI               @ call the write syscall (note: uses OABI calling conventions)
	cmp r0, #0                  @ syscall will return 0 if there is not an error
	bpl no_error                @ no error? skip (return 0)

	@handle error- take value, put in errno. return -1
	mov r4, r0                  @ copy return value from r0 to r4
	mov r0, #-1                 @ set our wrapper return value to -1
	EOR r4, r4, #0x80000000     @ negate the error value returned from the syscall (from negative to positive)
	ldr r5, =errno              @ get the address of the gloval variable "errno"
	str r4, [r5]                @ set errno = negated return value

no_error:
	ldmfd sp!, {r4, r5}         @ restore registers from the stack
	movs pc, lr					@return to calling function
