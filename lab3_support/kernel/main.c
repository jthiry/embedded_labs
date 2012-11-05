#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include "swi_handler.h"
#include "irq_handler.h"
#include "constants.h"
#include "kernel_util.h"
#include "c_kernel_util.h"

uint32_t global_data;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Wiring in Data abort handler */
	if (-1 == wire_exception_handler(EX_FABRT)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_FABRT));
	    return 0xbadc0de;
	}
	/* setup abort stack */
	setup_abort_stack();
	
	puts("Wiring in swi...\n");

	if (-1 == wire_exception_handler(EX_SWI)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_SWI));
	    return 0xbadc0de;
	}

	/* Wiring in irq handler */
//	if (-1 == wire_exception_handler(EX_IRQ)) {
//	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
//		    GET_EXP_VEC_ADDR(EX_FABRT));
//	    return 0xbadc0de;
//	}

	//Our code starts here
	



	puts("Setting up stack...\n");

//	initialize_timer();
	//Set up the stack
	unsigned* stack_ptr = setup_stack( START_STACK, argc, argv);
	//Start the user program
	puts("Starting user prog...\n");
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER );
	puts("--Exit steps complete\n");

	return status;
}
