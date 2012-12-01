/** @file mutex_create.S
 *
 * @brief mutex create sycall wrapper
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 * @author Hans Reichenbach <HansReich.gmail.com>
 * @author Josh Thiry <josh.thiry@gmail.com>
 *
 * @date   11/29/12
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_create.S"

FUNC(mutex_create)
  stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi MUTEX_CREATE              @ call the mutex create syscall. (note: uses OABI)
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
