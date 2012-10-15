@ crt0.S: Execution startup routine
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date

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


	
