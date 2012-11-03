/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/fileno.h>

	.file "time.S"

FUNC(time)
	stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi TIME_SWI                  @ call the time syscall. (note: uses OABI
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
