@ hello.S: Hello world! in assembly
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	   Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012
	
#include <bits/swi.h>
#include <bits/fileno.h>
	.file	"hello.S"
	.text

	.global	_hello
_hello:
	mov r0, #STDOUT_FILENO		@loading value for std output
	ldr r1, phello_str		@loading address of the string
	mov r2, #13			@moving the str len into r2

	@Params are set, so we are ready to call WRITE_SWI
	swi WRITE_SWI

	mov r0, #0			@move 0 into r0 so we can exit with status 0
	swi EXIT_SWI			@exiting

phello_str:
 	.word hello_str
 	.section .rodata

hello_str:
	.ascii "Hello World!\n"
