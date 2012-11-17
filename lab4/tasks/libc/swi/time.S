/** @file time.S
 *
 * @brief time sycall wrapper
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

	.file "time.S"

FUNC(time)
	stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi TIME_SWI                  @ call the time syscall. (note: uses OABI
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
