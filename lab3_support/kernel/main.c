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
	

	//Our code starts here
	initialize_timer();

//	unsigned *old_swi_data = malloc( sizeof(unsigned)*3 );
	unsigned *old_irq_data = malloc( sizeof(unsigned)*3 );

 	puts("DEBUG::main.c--about to wire in the swi handler\n");
/*
	//Wire in the SWI Handler
	install_handler( old_swi_data, (unsigned)S_HANDLER, (unsigned *)VECTOR_SWI );
	if(old_swi_data[0] == RET_BAD_CODE)
		return RET_BAD_CODE;
*/
	//Wire in the IRQ Handler
	install_handler( old_irq_data, (unsigned)R_HANDLER, (unsigned *)VECTOR_IRQ, (unsigned)OFFSET_IRQ_JUMP );
	if(old_irq_data[0] == RET_BAD_CODE)
		return RET_BAD_CODE;

	//Set up the stack
	unsigned* stack_ptr = setup_stack( START_STACK, argc, argv);

	//Start the user program
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER );
 	
	puts("DEBUG::main.c-- user_prog returned\n");

	//Unwire the Handlers
	//uninstall_handler( old_swi_data );
	uninstall_handler( old_irq_data );

//	free(old_swi_data);
	free(old_irq_data);

  	puts("DEBUG::main.c--after user prog\n");

	return status;
}
