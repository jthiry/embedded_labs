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

//prototype
int tasks_insertion_sort( task_t** tasks, size_t num_tasks );

int task_create(task_t* tasks , size_t num_tasks )
{
	//int i;
	//clear tcb... memset? loop?

	//check for insane input
	if( num_tasks > 62 ) return EINVAL;
	
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
	dispatch_nosave();	
	//dont return...
	return 1;
}

int event_wait(unsigned int dev  )
{
	//if(dev > NUM_DEVICES || dev < 0 ) return EINVAL; 	
	dev_wait(dev);
	
	//context switch to highest priority
	dispatch_nosave();	
	
	return 1;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  )
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
