#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include "swi_handler.h"
#include "irq_handler.h"
#include "constants.h"
#include "kernel_util.h"
#include "c_kernel_util.h"
#include <debug.h>

uint32_t global_data;
int debug_enabled = 1;


int kmain(int argc, char** argv, uint32_t table)
{
	if(debug_enabled==1)puts("Kernel initalization...\n");

	app_startup(); /* bss is valid after this point */
	global_data = table;
	
	if(debug_enabled==1)puts("Wiring in abort handler...\n");

	/* Wiring in Data abort handler */
	if (-1 == wire_exception_handler(EX_FABRT)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_FABRT));
	    return 0xbadc0de;
	}
	
	if(debug_enabled==1)puts("Setting up abort stack...\n");
	/* setup abort stack */
	setup_abort_stack();
	setup_irq_stack();
	//Our code starts here
	
	if(debug_enabled==1)puts("Wiring in swi handler...\n");
	
	if (-1 == wire_exception_handler(EX_SWI)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_SWI));
	    return 0xbadc0de;
	}
	
	if(debug_enabled==1)puts("Wiring in irq handler...\n");

	/* Wiring in irq handler */
	if (-1 == wire_exception_handler(EX_IRQ)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_FABRT));
	    return 0xbadc0de;
	}

	
	if(debug_enabled==1)puts("Initializing timers...\n");

	initialize_timer();


	if(debug_enabled==1)puts("Setting up stack...\n");

	//Set up the stack
	unsigned* stack_ptr = setup_stack( START_STACK, argc, argv);
	//Start the user program
	if(debug_enabled==1)puts("Starting user prog...\n");
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER );

	if(debug_enabled==1)puts("Restoring handlers...\n");
	restore_handlers();
	
	if(debug_enabled==1)puts("Exiting kernel.\n");

	return status;
}
