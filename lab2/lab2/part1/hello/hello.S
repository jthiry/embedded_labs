@ hello.S: Hello world! in assembly
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date
	
#include <bits/swi.h>
#include <bits/fileno.h>
	.file	"hello.S"
	.text

	.global	_start
_start:
	mov r0, #STDOUT_FILENO
	ldr r1, phello_str
	mov r2, #13
	swi WRITE_SWI
	mov r0, #0
	swi EXIT_SWI

phello_str:
 	.word hello_str
 	.section .rodata

hello_str:
	.ascii "Hello World!\n"
