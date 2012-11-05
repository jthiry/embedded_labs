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
	@WE ARE NOW IN SVC MODE
	@ we have the SVC sp, and our LR points to write.S
	STMFD sp!, {r0-r12,lr}   	  @ store the lr(write.S) onto the SVC stack
	MOV r1, sp		  @ pass the stack to the handler as an arg
	LDR r0, [lr, #-4]         @ load SWI into r0
	BIC r0,r0, #0xfffffff0	  @ get SWI # by bit-masking
	BL c_swi_handler
	str r0, [sp]
	LDMFD sp!, {r0-r12, lr}   @ unstack user's registers
	MOVS pc, lr               @ return from handler back to the user program
	@THE LAST INSTRUCTION SHOULD HAVE PUT US BACK INTO USER MODE
.type S_HANDLER, function
.size S_HANDLER, .-S_HANDLER
