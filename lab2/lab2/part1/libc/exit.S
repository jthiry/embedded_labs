@ exit.S: exit sycall wrapper
@
@ Authors: Joe Battaglia 1 <jabat295@gmail.com>
@          Hans Reiechenbach 2 <HansReich25@gmail.com>
@	   Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/errno.h>
#include <bits/swi.h>

	.file	"exit.S"
	.text

	.global	exit
exit:

	@r0 is the exit status coming in from C
	
	
	@put args into the syscall format

	@call syscall
	swi EXIT_SWI
	
