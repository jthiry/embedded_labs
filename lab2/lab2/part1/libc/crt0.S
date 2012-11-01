@ crt0.S: Execution startup routine
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/errno.h>
#include <bits/swi.h>
#include <bits/fileno.h>

	.file	"crt0.S"
	.text

	.global	_start
_start:
	ldmfd sp!, {r0,r1}     @ pull argc off the stack and put in r0
	mov r4, r0          @ put argc in r4 to use for branching
	
	subs r4, r4, #1     @ sub 1 from argc to see if it's the only arg
	ble done_args	      @ if it's the only one exit
	ldmfd sp!, {r1}     @ pull arg0 off the stack and put it in r1
	subs r4, r4, #1     @ sub 1 from argc to see if there are any args left
	ble done_args       @ if arg0 is the last one exit
	ldmfd sp!, {r2}     @ pull arg1 off the stack and put it in r2
	subs r4, r4, #1     @ sub 1 from argc to see if there are any args left
	ble done_args       @ if arg1 is the last one exit
	ldmfd sp!, {r3}     @ pull arg2 off the stack and put it in r3


	@remaining args stay on stack according to APCS
	
done_args:
	bl main             @ gets the address for the main function
	swi EXIT_SWI        @ exit the routine


	
