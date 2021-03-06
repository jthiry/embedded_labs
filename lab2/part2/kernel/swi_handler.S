@ S_HANDLER.S: 	contains assembly complement to kernel.c, which uses these functions to 
@				intercept swis and then branch to a user program
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012
	.file	"swi_handler.S"
	.text

@ this handler is branched to from the old swi handler by overwriting the first
@ 8 bytes with an instruction that jumps to this address. 
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
	BIC r0,r0,#0xfffffff0     @ get SWI # by bit-masking
	BL C_SWI_handler
	str r0, [sp]
	LDR r2, [sp, #14*4]
	MSR spsr, r2
	LDMFD sp!, {r0-r12, lr}   @ unstack user's registers
	ADD sp, sp, #4            @ remove space used to store SPSR
	MOVS pc, lr               @ return from handler back to the user program
.type S_HANDLER, function
.size S_HANDLER, .-S_HANDLER



	.global	ENABLE_USER_PROG
ENABLE_USER_PROG:
	STMFD sp!, {r1-r12, lr}   @ sp and lr are banked!!

	mov r1, #0xa3000000       @ init sp location
	sub r1, r1, #4            @svc sp gets stored here for later
	STR sp, [r1]              @sp we need to save
	sub r1, r1, #4            @link register goes here`for later
	add r2, pc, #12           @figure out where we should link (pc relative)

	str r2, [r1]              @ lr we need to save to return from user prog

	MSR   CPSR, #0xD0
	@user mode now!

	mov sp, r0                @ give user sp stack location

	LDR pc, =0xa2000000

	@return here after exit
	@switch back to kernel mode
ret:	@MSR   CPSR, #0xD3
	@r0 is return status
	mov r1, #0xa3000000
	sub r1, r1, #4
	LDR sp, [r1]
	LDMFD sp!, {r1-r12, lr}
	mov pc, lr

.type ENABLE_USER_PROG, function
.size ENABLE_USER_PROG, .-ENABLE_USER_PROG

	

	.global CHECK_INST
CHECK_INST:
	STMFD sp!, {r1-r12, lr}		@store registers and lr
	BIC r0, r0, #0xff		@bit clear to make sure instruction correct
	BIC r0, r0, #0xf00		@bit clear reast of instruction
	LDMFD sp!, {r1-r12, lr}		@load back registers
	mov pc, lr			@branch back to kernel.c

.type CHECK_INST, function
.size CHECK_INST, .-CHECK_INST

	.global GET_IMM
GET_IMM:
	STMFD sp!, {r1-r12, lr}
	ldr r1, =0xFFFFF000
	BIC r0, r0, r1 
	LDMFD sp!, {r1-r12, lr}
	mov pc, lr

.type GET_IMM, function
.size GET_IMM, .-GET_IMM
