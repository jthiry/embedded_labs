@ hello.S: Hello world! in assembly
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012

#include <bits/swi.h>
#include <bits/fileno.h>
	.file	"exit.S"
	.text

	.global	_EXIT

_EXIT:

  mov r1, #0xa3000000   @start at a3million to find return mem addr
  sub pc, r1, #8        @sub 8 from a3million for proper address to find
	@send program to the memory address given

.type _EXIT, function
.size _EXIT, .-_EXIT
