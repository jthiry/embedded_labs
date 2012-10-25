	
	.file	"swi_handler.S"
	.text

	.global	S_HANDLER
S_HANDLER:
	SUB sp, sp, #4 @ leave room on stack for SPSR 
	STMFD sp!, {r0-r12, lr} @ store user's gp registers
	MRS r2, spsr
	STR r2, [sp, #14*4]
	MOV r1, sp
	LDR r0, [lr, #-4]
	BIC r0,r0,#0xff000000 @ get SWI # by bit-masking
	BL C_SWI_handler 
	LDR r2, [sp, #14*4]
	MSR spsr, r2
	LDMFD sp!, {r0-r12, lr} @ unstack user's registers
	ADD sp, sp, #4 @ remove space used to store SPSR 
	MOVS pc, lr @ return from handler
.type S_HANDLER, function
.size S_HANDLER, .-S_HANDLER



	.global	ENABLE_USER_PROG
ENABLE_USER_PROG:
	STMFD sp!, {r0-r12} @ sp and lr are banked!!
	MSR   CPSR, #0xD0
	@user mode now!,

	mov r1, #0xa3000000
inc:	sub r1, r1, #4
	subs r0, r0, #1
	bne inc
	 
	mov sp, r1

	ldr lr, [pc]
	LDR pc, =0xa2000000
	
	@switch back to kernel mode
	MSR   CPSR, #0xD3
	LDMFD sp!, {r0-r12}
	movs pc, lr

.type ENABLE_USER_PROG, function
.size ENABLE_USER_PROG, .-ENABLE_USER_PROG
