
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
	MOVS pc, lr @ return from handler back to the user program
.type S_HANDLER, function
.size S_HANDLER, .-S_HANDLER



	.global	ENABLE_USER_PROG
ENABLE_USER_PROG:
	STMFD sp!, {r1-r12, lr} @ sp and lr are banked!!

	mov r1, #0xa3000000 @ init sp location
	sub r1, r1, #4 @svc sp gets stored here for later
	STR sp, [r1] @sp we need to save
	sub r1, r1, #4 @link register goes here`for later
	add r2, pc, #12 @figure out where we should link (pc relative)

	str r2, [r1] @ lr we need to save to return from user prog	

	MSR   CPSR, #0xD0
	@user mode now!

	mov sp, r0 @ give user sp stack location

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

	.global	ghetto_exit
ghetto_exit:
	mov r1, #0xa3000000
	sub r1, r1, #8
	LDR pc, [r1]
.type ghetto_exit, function
.size ghetto_exit, .-ghetto_exit
