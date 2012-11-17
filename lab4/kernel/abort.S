/**
 * @file   abort.S
 * @author Chinmay Kamat <chinmaykamat@gmail.com>
 * @date   Sat, 27 October 2012 14:49:50 EDT
 *
 * @brief  Contains code for the abort handler of Gravel kernel.
 *         Calls register_dump to dump the registers
 */
#include <asm.h>

	.file   "abort.S"

FUNC(abort_handler)
	/* push contents of registers r0-r12 and lr on stack*/
	stmfd sp!, {r0-r12, lr}
	/* Save cpsr and spsr */
	mrs r12, cpsr
	mrs r6, spsr
	/* Go to mode which originated abort */
	mov r7, r6
	/* If in user mode, transition to SYS */
	tst r6, #0xF
	orreq r6, r6, #0xF
	msr cpsr, r6
	/* Save lr and sp and (r8_fiq to r12_fiq) of mode */
	mov r0, r8
	mov r1, r9
	mov r2, r10
	mov r3, r11
	mov r4, r12
	mov r5, lr
	mov r6, sp
	/* Come back to abrt mode */
	msr cpsr, r12
	/* Save remainder */
	stmfd sp!, {r0-r7}
	/* Pass parameters through stack */
	mov r0, sp
	/*
	 * Branch to register dump function with context as param
	 */
	b register_dump
