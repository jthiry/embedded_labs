/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */


#include "exit.h"
#include <bits/swi.h>
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
#include <lock.h>


//exits the kernel with a given status
void c_exit(int status) { _exit(status);}



//call the appropriate method based on the swi
int c_swi_handler(unsigned swi_num, unsigned * regs){

	if(debug_enabled ==1) printf ("c_swi_handler:: swi_num = %d\n", swi_num ) ;
	switch(swi_num){

		case EXIT_SWI:
			c_exit(regs[0]);
			break;

		case READ_SWI:
			return read_syscall(regs[0], (void *) regs[1], regs[2]);

		case WRITE_SWI:
			return write_syscall(regs[0], (void *) regs[1], regs[2]);

		case TIME_SWI:
			return time_syscall();

		case SLEEP_SWI:
			sleep_syscall(regs[0]);
			break;
		
		case CREATE_SWI:
			return task_create((task_t*)regs[0], regs[1]);

		case MUTEX_CREATE:
			return mutex_create();

		case MUTEX_LOCK:
			return mutex_lock(regs[0]);

		case MUTEX_UNLOCK:
			return mutex_unlock(regs[0]);

		case EVENT_WAIT:
			return event_wait(regs[0]);

		default:
			invalid_syscall(0x0badc0de);
			break;
	}
	return 0;
}
