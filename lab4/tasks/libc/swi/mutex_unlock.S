/** @file mutex_unlock.S
 *
 * @brief mutex unlock sycall wrapper
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 * @author Hans Reichenbach <HansReich.gmail.com>
 * @author Josh Thiry <josh.thiry@gmail.com>
 *
 * @date   11/29/12
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_unlock.S"

FUNC(mutex_unlock)
  stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi MUTEX_UNLOCK              @ call the mutex unlock syscall. (note: uses OABI)
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
