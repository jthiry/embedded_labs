@ crt0.S: Execution startup routine
@
@ Authors: Joe Bataglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	   Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/errno.h>
#include <bits/swi.h>
#include <bits/fileno.h>

	.file	"crt0.S"
	.text

	.global	_start
_start:
	ldmfd sp!, {r0} @ r0  = argc
	mov r4, r0
	
	subs r4, r4, #1
	ble done_args	
	ldmfd sp!, {r1} @ r1 = arg0
	subs r4, r4, #1
	ble done_args
	ldmfd sp!, {r2} @ r2 = arg1
	subs r4, r4, #1
	ble done_args
	ldmfd sp!, {r3} @ r3 = arg2


	@remaining args stay on stack according to APCS
	
done_args:
	bl main
	swi EXIT_SWI


	
