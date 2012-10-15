@ exit.S: exit sycall wrapper
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date

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
	
