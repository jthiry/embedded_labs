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
#include "include/constants.h"
#include "kernel_util.h"
#include "c_kernel_util.h"

int main(int argc, char *argv[]) {


	//Wire in the SWI Handler
	unsigned *old_instr = malloc( 4*3 );
	install_handler( old_instr, (unsigned)S_HANDLER, (unsigned *)VECTOR_SWI );
	if(old_instr[0] == RET_BAD_CODE)
		return RET_BAD_CODE;

	//Set up the stack
	unsigned* stack_ptr = setup_stack(START_STACK, argc, argv);
	
	//Start the user program
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER);

	//Unwire the SWI Handler
	uninstall_handler( old_instr );
	
	free(old_instr);

	return status;
}

