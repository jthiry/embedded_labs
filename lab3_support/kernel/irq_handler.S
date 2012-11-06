@ R_HANDLER.S: 	contains assembly complement to kernel.c, which uses these functions to 
@				intercept irqs and then branch to a user program
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012
	.file	"irq_handler.S"
	.text
 
@ this irq handler stores the registers, extracts the irq number, and then passes it to our C_irq_handler
@ which handles it. Once the irq is handled we branch back to the user program


	.global	R_HANDLER
R_HANDLER:
	@WE ARE NOW IN SVC MODE
	@ we have the SVC sp, and our LR points to write.S
	STMFD sp!, {r0-r12,lr}   	  @ store the lr(write.S) onto the SVC stack
	BL c_irq_handler
	LDMFD sp!, {r0-r12, lr}   @ unstack user's registers
	MOVS pc, lr               @ return from handler back to the user program
	@THE LAST INSTRUCTION SHOULD HAVE PUT US BACK INTO USER MODE
.type R_HANDLER, function
.size R_HANDLER, .-R_HANDLER


