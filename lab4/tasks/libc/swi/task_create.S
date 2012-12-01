/** @file task_create.S
 *
 * @brief task_create sycall wrapper
 *
 * @author Joe Battaglia <JABAT295@gmail.com>
 *         Hans Reichenbach <HansReich25@gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * @date 2012-11-29
 */

#include <asm.h>
#include <bits/swi.h>
#include <bits/fileno.h>


        .file "task_create.S"

FUNC(task_create)
        stmfd sp!, {r4-r12, lr}       @ store r4 on the stack so we don't crush someone
        swi CREATE_SWI                  @ call the time syscall. (note: uses OABI
        ldmfd sp!, {r4-r12, pc}^      @ restore registers from the stack

