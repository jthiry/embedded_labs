@ S_HANDLER.S: 	contains assembly complement to kernel.c, which uses these functions to 
@				intercept swis and then branch to a user program
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012
	.file	"swi_handler.S"
	.text
 
@ this swi handler stores the registers, extracts the swi number, and then passes it to our C_swi_handler
@ which handles it. Once the swi is handles we branch back to the user program


	.global	S_HANDLER
S_HANDLER:
	SUB sp, sp, #4            @ leave room on stack for SPSR
	STMFD sp!, {r0-r12, lr}   @ store user's gp registers
	MRS r2, spsr
	STR r2, [sp, #14*4]       @ store the spsr on the stack
	MOV r1, sp				  @ pass the stack to the handler as an arg	
	LDR r0, [lr, #-4]         @ load  
	BIC r0,r0, #0xfffffff0	  @ get SWI # by bit-masking
	BL C_SWI_handler
	str r0, [sp]
	LDR r2, [sp, #14*4]
	MSR spsr, r2
	LDMFD sp!, {r0-r12, lr}   @ unstack user's registers
	ADD sp, sp, #4            @ remove space used to store SPSR
	MOVS pc, lr               @ return from handler back to the user program
.type S_HANDLER, function
.size S_HANDLER, .-S_HANDLER


