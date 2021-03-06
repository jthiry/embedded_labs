/**
 * @file   exception.c
 * @author Chinmay Kamat <chinmaykamat@cmu.com>
 * @date   Sat, 27 October 2012 05:18:23 EDT
 *
 * @brief  Contains C functions to handle exceptions
 */

#include <types.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <bits/swi.h>
#include <exports.h>
#include <assert.h>
#include "swi_handler.h"
#include "c_swi_handler.h"
#include "irq_handler.h"
#include <debug.h>

#define PC_OFFSET 0x8
#define LDR_PC_INSTRUCTION 0xe51ff000 /* ldr pc, [pc, #-0] == 0xe51ff000 */
#define POSITIVE_LDR_INSTRUCTION_MASK 0x00800FFF
/* Mask offset bits to 1s and set bit indicating positive offset */

#define ABT_STACK_SIZE 256
#define IRQ_STACK_SIZE 256


/* Globals */
static uint32_t *uboot_abt_address;
static uint32_t *uboot_swi_address;
static uint32_t *uboot_irq_address;
static uint32_t uboot_swi_ins[2];
static uint32_t uboot_irq_ins[2];
static uint32_t uboot_abt_ins[2];
/* Exported from main - stores r8 */
extern uint32_t global_data;
/* Setting up IRQ stack in bss of kernel */
uint32_t abort_stack[ABT_STACK_SIZE];
uint32_t irq_stack[IRQ_STACK_SIZE];

int wire_exception_handler(unsigned exception)
{
	unsigned *exception_vector;
	unsigned *jumptable_address;
	unsigned *uboot_exception_address;
	int relative_offset;
	exception_vector = (unsigned *) GET_EXP_VEC_ADDR(exception);

	if (((*exception_vector | POSITIVE_LDR_INSTRUCTION_MASK)
	     ^ (LDR_PC_INSTRUCTION | POSITIVE_LDR_INSTRUCTION_MASK)) != 0) {
		/*
		 * Mask exception_vector and LDR_PC_INSTRUCTION with
		 * POSITIVE_LDR_INSTRUCTION mask to account for ldr instruction
		 * with any negative or positive offset.
		 * Check if ldr instruction found at exception.
		 * else return error */
		return -1;
	}
	/*
	 * Value at 0x8 is 0xe59ff014 == ldr pc, [pc, #0x14]
	 */
	relative_offset = *exception_vector & 0xFFF; /* extract relative offset */

	/*
	 * pc value at this stage is exception vector + 0x8
	 * Correcting offset gives exact jump location for exception handler
	 */
	jumptable_address = (unsigned *) (relative_offset
					  + GET_EXP_VEC_ADDR(exception)
					  + PC_OFFSET);

	uboot_exception_address = (unsigned int *) *jumptable_address;

	//puts("exception.c::wire_handler:: address dump\n");
	//hexdump(uboot_exception_address, 16);
	if (exception == EX_FABRT) {
		/* Save earlier values and wire in our exception handler */
		uboot_abt_address = uboot_exception_address;
		uboot_abt_ins[0] = *uboot_exception_address;
		*uboot_exception_address = LDR_PC_INSTRUCTION | 0x4;
		uboot_exception_address++;
		uboot_abt_ins[1] = *uboot_exception_address;
		/* Move address of our abort handler here */
		*uboot_exception_address = (unsigned int) abort_handler;

	}
	if (exception == EX_SWI) {
		/* Save earlier values and wire in our exception handler */
		uboot_swi_address = uboot_exception_address;
		uboot_swi_ins[0] = *uboot_exception_address;
		*uboot_exception_address = LDR_PC_INSTRUCTION | 0x4;
		uboot_exception_address++;
		uboot_swi_ins[1] = *uboot_exception_address;
		/* Move address of our abort handler here */
		*uboot_exception_address = (unsigned int) S_HANDLER;

	}

	if (exception == EX_IRQ) {
		/* Save earlier values and wire in our exception handler */
		uboot_irq_address = uboot_exception_address;
		uboot_irq_ins[0] = *uboot_exception_address;
		*uboot_exception_address = LDR_PC_INSTRUCTION | 0x4;
		uboot_exception_address++;
		uboot_irq_ins[1] = *uboot_exception_address;
		/* Move address of our abort handler here */
		*uboot_exception_address = (unsigned int) R_HANDLER;

	}

	//puts("exception.c::wire_handler:: post dump\n");
	//hexdump(uboot_exception_address, 16);
	return 0;
}

void restore_handlers()
{
	if(uboot_irq_address != 0)
	{
		if(debug_enabled==1)
			puts("uninstalling irq...\n");
		*uboot_irq_address = uboot_irq_ins[0];
		uboot_irq_address++;
		*uboot_irq_address = uboot_irq_ins[1];
	}
	if(uboot_swi_address != 0)
	{
		if(debug_enabled==1)
			puts("uninstalling swi...\n");
		*uboot_swi_address = uboot_swi_ins[0];
		uboot_swi_address++;
		*uboot_swi_address = uboot_swi_ins[1];
	}
	if(uboot_abt_address != 0)
	{
		if(debug_enabled==1)
			puts("uninstalling abt...\n");
		*uboot_abt_address = uboot_abt_ins[0];
		uboot_abt_address++;
		*uboot_abt_address = uboot_abt_ins[1];
	}
}


void setup_irq_stack() {
	//TODO

	uint32_t cpsr;

	// save cpsr value before entering IRQ mode
	cpsr = read_cpsr();
	// Change to IRQ mode with IRQ and FIQs disabled
	write_cpsr(PSR_MODE_IRQ | PSR_IRQ | PSR_FIQ);
	// Setup ABT mode stack
	asm volatile (
		"mov sp, %0\n"
		:
		:"r"(&irq_stack[IRQ_STACK_SIZE - 1])
		);
	///Back to SVC
	write_cpsr(cpsr);

	return;
}

void setup_user_stack() {
	//TODO
	/*
	uint32_t cpsr;

	// save cpsr value before entering IRQ mode
	cpsr = read_cpsr();
	// Change to IRQ mode with IRQ and FIQs disabled
	write_cpsr(PSR_MODE_ABT | PSR_IRQ | PSR_FIQ);
	// Setup ABT mode stack
	asm volatile (
		"mov sp, %0\n"
		:
		:"r"(&abort_stack[ABT_STACK_SIZE - 1])
		);
	///Back to SVC
	write_cpsr(cpsr);
	*/
	return;
}

void setup_abort_stack() {
	uint32_t cpsr;

	/* save cpsr value before entering IRQ mode */
	cpsr = read_cpsr();
	/* Change to IRQ mode with IRQ and FIQs disabled */
	write_cpsr(PSR_MODE_ABT | PSR_IRQ | PSR_FIQ);
	/* Setup ABT mode stack */
	asm volatile (
		"mov sp, %0\n"
		:
		:"r"(&abort_stack[ABT_STACK_SIZE - 1])
		);
	/* Back to SVC */
	write_cpsr(cpsr);
	return;
}
