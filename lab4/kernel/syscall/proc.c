/* @file proc.c
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
#include <debug.h>

//prototype
int tasks_insertion_sort( task_t** tasks, size_t num_tasks );

int task_create(task_t* tasks , size_t num_tasks )
{
	disable_interrupts();

	if(debug_enabled == 1)puts("task_create++\n");
	//int i
	//clear tcb... memset? loop?

	//check for insane input
	if( num_tasks > (size_t)62 ){
		printf("EINVAL = %d", EINVAL); 
		return -EINVAL;
	}
	//lamda in bounds
	/*
	for( i = 0; i < num_tasks; i++)
	{



	}
	*/

	//verify that they are schedulable, and sort
	//if ( assign_schedule( tasks, num_tasks) == 0 ) return ESCHED;//error. unschedulable

	//allocate_tasks
	allocate_tasks( &tasks, num_tasks );

	//context switch to highest priority
	if(debug_enabled == 1)puts("task_create...dispatching\n");
	dispatch_nosave();

	enable_interrupts();
	//dont return...
	return 1;
}

int event_wait(unsigned int dev  )
{
	disable_interrupts();
	//if(dev > (unsigned int) NUM_DEVICES ) return -EINVAL;
	if(debug_enabled == 1)puts("event wait++\n");
	dev_wait(dev);

	if(debug_enabled == 1)puts("event wait...dispatching_sleep\n");
	//context switch to highest priority
	dispatch_sleep();
	//if(debug_enabled == 1)puts("event wait--\n");

	enable_interrupts();

	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  )
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
