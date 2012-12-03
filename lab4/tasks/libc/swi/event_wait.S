/** @file event_wait.S
 *
 * @brief time sycall wrapper
 *
 * @author Joe Battaglia <JABAT295@gmail.com>
 * 	   Hans Reichenbach <HansReich25@gmail.com>
 * 	   Josh Thiry <josh.thiry@gmail.com>
 * @date 2012-11-29
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/fileno.h>


	.file "event_wait.S"

FUNC(event_wait)
	stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
        swi EVENT_WAIT                  @ call the time syscall. (note: uses OABI
        @ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack
	mov pc, lr
