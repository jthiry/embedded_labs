/** @file mutex_lock.S
 *
 * @brief mutex lock sycall wrapper
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 * @author Hans Reichenbach <HansReich.gmail.com>
 * @author Josh Thiry <josh.thiry@gmail.com>
 *
 * @date   11/29/12
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_lock.S"

FUNC(mutex_lock)
  stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
	swi MUTEX_LOCK                @ call the mutex lock syscall. (note: uses OABI)
	ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
