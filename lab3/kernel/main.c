#include <exports.h>


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

	initialize_timer();

	unsigned *old_swi_data = malloc( sizeof(unsigned)*3 );
	unsigned *old_irq_data = malloc( sizeof(unsigned)*3 );

 	puts("DEBUG--about to wire in the swi handler in kernel\n");

	//Wire in the SWI Handler
	install_handler( old_swi_data, (unsigned)S_HANDLER, (unsigned *)VECTOR_SWI );
	if(old_swi_data[0] == RET_BAD_CODE)
		return RET_BAD_CODE;

	//Wire in the IRQ Handler
/*	install_handler( old_irq_data, (unsigned)R_HANDLER, (unsigned *)VECTOR_IRQ );*/
/*	if(old_irq_data[0] == RET_BAD_CODE)*/
/*		return RET_BAD_CODE;*/

	//Set up the stack
	unsigned* stack_ptr = setup_stack( START_STACK, argc, argv);

	//Start the user program
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER );

	//Unwire the Handlers
	uninstall_handler( old_swi_data );
	uninstall_handler( old_irq_data );

	free(old_swi_data);
	free(old_irq_data);

  	puts("DEBUG--after user prog in kernel\n");

	return status;
}
