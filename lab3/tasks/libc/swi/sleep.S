/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/fileno.h>

	.file "sleep.S"

FUNC(sleep)
	stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi SLEEP_SWI                  @ call the sleep syscall. (note: uses OABI
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
