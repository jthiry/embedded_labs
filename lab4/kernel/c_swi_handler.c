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
#include <debug.h>
#include <syscall.h>
#include <lock.h>
#include <sched.h>
#include <arm/exception.h>


//exits the kernel with a given status
void c_exit(int status) { _exit(status);}



//call the appropriate method based on the swi
int c_swi_handler(unsigned swi_num, unsigned * regs){
	
	int ret = 0;

	switch(swi_num){

		case EXIT_SWI:
			c_exit(regs[0]);
			break;

		case READ_SWI:
			ret = read_syscall(regs[0], (void *) regs[1], regs[2]);
			break;

		case WRITE_SWI:
			ret = write_syscall(regs[0], (void *) regs[1], regs[2]);
			break;

		case TIME_SWI:
			ret = time_syscall();
			break;

		case SLEEP_SWI:
			sleep_syscall(regs[0]);
			break;

		case CREATE_SWI:
			ret = task_create((task_t*)regs[0], regs[1]);
			break;

		case MUTEX_CREATE:
			ret = mutex_create();
			break;

		case MUTEX_LOCK:
			ret = mutex_lock(regs[0]);
			break;

		case MUTEX_UNLOCK:
			ret = mutex_unlock(regs[0]);
			break;

		case EVENT_WAIT:
			ret = event_wait(regs[0]);
			break;

		default:
			invalid_syscall(0x0badc0de);
			break;
	}

	
	return ret;
}
