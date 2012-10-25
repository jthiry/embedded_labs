	.file	"swi_handler.S"
	.text

	.global	_S_HANDLER
_S_HANDLER:
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
