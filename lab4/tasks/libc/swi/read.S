@ read.S: read syscall wrapper
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/swi.h>
#include <bits/fileno.h>
#include <asm.h>

	.file	"read.S"

FUNC(read)
	swi READ_SWI                  @ call the read syscall. (note: uses OABI
	cmp r0, #0                    @ return 0 if no error
	bpl no_error                  @ skip code if no error

  @ handle error
	mov r1, r0                    @ store error num returned to our variable.
	mov r0, #-1                   @ set return to -1
	rsb r1, r1, #0       	      @ flip sign of err num from syscall to positive
	ldr r2, =errno                @ get the address of the global variable errno
	str r1, [r2]                  @ set errno to positive of syscall return

no_error:
	mov pc, lr			      @ return to the calling function
