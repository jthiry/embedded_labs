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
	swi MUTEX_UNLOCK                   @ call the write syscall. (note: uses OABI
        cmp r0, #0                      @ return 0 if no error
        bpl no_error                    @ skip code if no error

	@ handle error
        mov r1, r0                    @ store error num returned to our variable.
        mov r0, #-1                   @ set return to -1
        rsb r1, r1, #0                @ flip sign of err num from syscall to positive
        ldr r2, =errno                @ get the address of the global variable errno
        str r1, [r2]                  @ set errno to positive of syscall return

no_error:
        mov pc, lr

