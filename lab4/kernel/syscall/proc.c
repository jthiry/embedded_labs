** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

//prototype
int tasks_insertion_sort( task_t** tasks, size_t num_tasks );

int task_create(task_t* tasks , size_t num_tasks )
{
	
	//clear tcb... memset? loop?

	//check for insane input
		//num < 64
		//lamda in bounds

	//verify that they are schedulable, and sort
	if ( assign_schedule( tasks, num_task) > 0 ) ;
	else return 1;//error. unschedulable
	
	//begin to schedule them
	//??	

	//allocate_tasks
	allocate_tasks( tasks, num_tasks )

	//context switch to highest priority
	dispatch_nosave();	
	
	
	//dont return...

  return 1; /* remove this line after adding your code */
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
  return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}

