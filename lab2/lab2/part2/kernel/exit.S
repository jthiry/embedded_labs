@ exit.S: This is used by the C_swi_handler to exit from the user program and return to the kernel
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/swi.h>
#include <bits/fileno.h>
.equ STACK_START, 0xa3000000
.equ LR_OFFSET, 8

	.file	"exit.S"
	.text

	.global	_EXIT

_EXIT:

  mov r1, #STACK_START          @start at a3million to find return mem addr
  sub r1, r1, #LR_OFFSET        @sub 8 from a3million for proper address to find
  ldr pc, [r1]
	@send program to the memory address given

.type _EXIT, function
.size _EXIT, .-_EXIT
