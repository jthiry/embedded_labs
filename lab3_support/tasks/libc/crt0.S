/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>
#include "bits/swi.h"

	.file "crt0.S"

FUNC(_start)
	ldmfd sp!, {r0}     @ pull argc off the stack and put in r0
	mov r1, sp          @ put the address of argv into r1
	


	
done_args:
	bl main             @ gets the address for the main function
	swi EXIT_SWI        @ exit the routine
