/** @file swi_handler.S
 *
 * @brief SWI handler function
 *
 * @date   2008-11-17
 */

#include <asm.h>
#include <arm/psr.h>

	.file   "swi_handler.S"

/**
 * @brief Calls the SWI dispatcher with the SWI number and returns to user mode.
 *
 * We now need to save spsr with interrupt disabled because spsr is considered
 * a scratch register to assist IRQ stack transplant.
 */
FUNC(S_HANDLER)
	/* Save common and svc registers. */
	stmfd   sp!, {r0-r12, lr}
	/* Remember the stack location to give the context to the SWI dispatcher */
	mov     r1, sp
	/* Save user mode registers. */
	mrs     r0, spsr
	stmfd   sp, {r0, sp, lr}^
	add     sp, sp, #-12

	/* Re-enable all interrupts. */
	msr     cpsr_c, #(PSR_MODE_SVC)

	/* Restore jump table in r8. */
	ldr     ip, =global_data
	ldr     r8, [ip]

	/* Determine the SWI number and call the dispatch function. */
	ldr     r0, [lr, #-4]
	ldr     ip, =0xffffff00
	bic     r0, r0, ip
	/* r1 has already been correctly loaded above */
	bl      c_swi_handler

	/* Disable all interrupts. */
	msr     cpsr_c, #(PSR_MODE_SVC | PSR_IRQ)

	/* Restore user registers. */
	ldmfd   sp, {r0, sp, lr}^
	add     sp, sp, #12
	msr     spsr, r0
	/* Return to user mode. */
	ldmfd   sp!, {r0-r12, pc}^
