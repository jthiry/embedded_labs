/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Joe Battaglia <jabat295@gmail.com>
 *         Hans Reichenbach <HansReich25@gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 *
 * @date 11/16/12
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/fileno.h>

	.file "sleep.S"

FUNC(sleep)
	stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi SLEEP_SWI                  @ call the sleep syscall. (note: uses OABI
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
