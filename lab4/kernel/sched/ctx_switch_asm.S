/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 *  @author  Joe Battaglia <JABAT295.gmail.com>
 *          Hans Reichenbach <HansReich.gmail.com>
 *          Josh Thiry <josh.thiry@gmail.com>
 * @date   2012-11-29
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)
	/*Save the stack pointer */
	/* Save the registers and return address */
	STR   r4, [r1], #4
	STR   r5, [r1], #4
	STR   r6, [r1], #4
	STR   r7, [r1], #4
	STR   r8, [r1], #4
	STR   r9, [r1], #4
	STR   r10, [r1], #4
	STR   r11, [r1], #4
	STR   sp, [r1], #4
	STR   lr, [r1], #4

	/* check if this task has been run*/
	LDR   r4, [r0, #40]
	LDR   r5, =0xfeedbeef
	CMP   r4, r5
	BEq   ctx_switch_half

  /* branch to the half ctx switch to load the new process*/
	/* Load the registers from the new process */
	LDR   r4, [r0], #4
	LDR   r5, [r0], #4
	LDR   r6, [r0], #4
	LDR   r7, [r0], #4
	LDR   r8, [r0], #4
	LDR   r9, [r0], #4
	LDR   r10, [r0], #4
	LDR   r11, [r0], #4
	/* load the stack pointer from the new process*/
  LDR   sp, [r0], #4
  /* return to where the new process was*/
  LDR   lr, [r0], #4
  MOV   pc, lr



/*add your code to perform a half context switch */
FUNC(ctx_switch_half)
	/* r0 points to target context? */
	LDR   r4, [r0], #4
	LDR   r5, [r0], #4
	LDR   r6, [r0], #4

	B     launch_task
