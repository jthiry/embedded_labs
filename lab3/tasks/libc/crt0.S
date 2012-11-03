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
	ldmfd sp!, {r0}     @ pull argc off the stack and put in r0
	mov r1, sp          @ put the address of argv into r1
	


	
done_args:
	bl main             @ gets the address for the main function
	swi EXIT_SWI        @ exit the routine


	
