/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */


#include "exit.h"
#include "constants.h"
#include <exports.h>
#include "bits/fileno.h"
#include "bits/errno.h"
#include "include/arm/interrupt.h"
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include "include/arm/timer.h"
#include <debug.h>
#include <syscall.h>


//exits the kernel with a given status
void c_exit(int status) { _exit(status);}



//call the appropriate method based on the swi
int c_swi_handler(unsigned swi_num, unsigned * regs){

	if(debug_enabled ==1) printf ("c_swi_handler:: swi_num = %d\n", swi_num ) ;
	switch(swi_num){

		case SWI_NUM_EXIT:
			c_exit(regs[0]);
			break;

		case SWI_NUM_READ:
			return read_syscall(regs[0], (void *) regs[1], regs[2]);

		case SWI_NUM_WRITE:
			return write_syscall(regs[0], (void *) regs[1], regs[2]);

		case SWI_NUM_TIME:
			return time_syscall();

		case SWI_NUM_SLEEP:
			sleep_syscall(regs[0]);
			break;

		default:
			invalid_syscall(RET_BAD_CODE);
			break;
	}
	return 0;
}
