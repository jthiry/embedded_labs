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
	swi TIME_SWI                   @ call the write syscall. (note: uses OABI
        cmp r0, #0                      @ return 0 if no error
        bpl no_error                    @ skip code if no error

  @ handle error
        mov r4, r0                      @ store error num returned to our variable.
        mov r0, #-1                     @ set return to -1
        EOR r4, r4, #0x80000000         @ flip sign of error num from syscall to positive
        ldr r5, =errno                  @ get the address of the global variable "errno"
        str r4, [r5]                    @ set errno to positive of syscall return

no_error:
        mov pc, lr

