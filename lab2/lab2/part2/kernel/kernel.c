/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

#include <exports.h>
#include "swi_handler.h"
#include "constants.h"
#include "kernel_util.h"

int main(int argc, char *argv[]) {


	//Wire in the SWI Handler
	unsigned old_instr[3] = install_handler( (unsigned)S_HANDLER, VECTOR_SWI );

	//Set up the stack
	unsigned stack_ptr = setup_stack(STACK_START, argc, argv);
	
	//Start the user program
	int status = _enable_user_prog( START_LOCATION, (unsigned)stack_ptr);

	//Unwire the SWI Handler
	uninstall_handler( old_instr, VECTOR_SWI );

	return status;
}

