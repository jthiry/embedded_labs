/**
 * @file assert.c
 *
 * @brief Assertion and debugging infrastructure.
 *
 * @author Kartik Subramanian
 */

#include <assert.h>
#include <stdarg.h>
#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>

void panic(const char* fmt, ...)
{
	va_list list;

	va_start(list, fmt);
	// XXX This is not working... fix it later.
	//vprintf(fmt, list);
	printf("PANIC!");
	va_end(list);

	disable_interrupts();

	while(1);
}

/* reg dump method for debug purposes*/
void register_dump(struct context *reg_context)
{
	printf("PREFETCH ABORT - caused by pc = 0x%08x\n",
	       reg_context->lr_abt - 4);
	printf("Registers:\n"
	       "r0: 0x%08x, r1: 0x%08x, r2: 0x%08x, r3: 0x%08x,\n"
	       "r4: 0x%08x, r5: 0x%08x, r6: 0x%08x, r7: 0x%08x,\n",
	       reg_context->r0, reg_context->r1, reg_context->r2,
	       reg_context->r3, reg_context->r4, reg_context->r5,
	       reg_context->r6, reg_context->r7);
	if ((reg_context->cpsr & PSR_MODE_FIQ) == PSR_MODE_FIQ) {
		printf("r8: 0x%08x, r9: 0x%08x, r10: 0x%08x, r11: 0x%08x,\n"
		       "r12: 0x%08x, ",
		       reg_context->r8_fiq, reg_context->r9_fiq,
		       reg_context->r10_fiq, reg_context->r11_fiq,
		       reg_context->r12_fiq);
	} else {
		printf("r8: 0x%08x, r9: 0x%08x, r10: 0x%08x, r11: 0x%08x,\n"
		       "r12: 0x%08x, ",
		       reg_context->r8, reg_context->r9,
		       reg_context->r10, reg_context->r11,
		       reg_context->r12);
	}
	printf("lr: 0x%08x, sp: 0x%08x, cpsr: 0x%08x\n",
	       reg_context->lr, reg_context->sp, reg_context->cpsr);
	while(1);
}
