#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include "swi_handler.h"
#include "constants.h"
#include "kernel_util.h"
#include "c_kernel_util.h"

uint32_t global_data;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Add your code here */

	//Wire in the SWI Handler
	unsigned *old_instr = malloc( 4*3 );
	install_handler( old_instr, (unsigned)S_HANDLER, (unsigned *)VECTOR_SWI );
	if(old_instr[0] == RET_BAD_CODE)
		return RET_BAD_CODE;

	//Set up the stack
	unsigned* stack_ptr = setup_stack( START_STACK, argc, argv);
	
	//Start the user program
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER );

	//Unwire the SWI Handler
	uninstall_handler( old_instr );
	
	free(old_instr);

	return status;
}
