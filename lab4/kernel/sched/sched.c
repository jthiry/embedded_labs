/** @file sched.c
 *
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>
#include <debug.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */



/*
* ????
*/
void sched_init(task_t* main_task  __attribute__((unused)))
{

}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */

static void idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  , size_t num_tasks  )
{
	if(debug_enabled == 1)puts("allocate_tasks...++\n");

	//clear the run queue
	runqueue_init();

	//set up TCBs for all tasks
	//put the task in runqueue
	//set up the TCB for idle task
	//make idle task schedulable

	size_t i;
	tcb_t* cur_tcb; 
	task_t* cur_task;;
	//for each task
	if(debug_enabled == 1)puts("allocate_tasks...entering loop\n");
	for( i = 0; i < num_tasks; i++)
	{
		if(debug_enabled == 1)printf("allocate_tasks...allocating task %d\n", (unsigned)i);
		cur_tcb = &system_tcb[i+1];
		cur_task = tasks[i];

		//create its tcb entry
		cur_tcb->native_prio = i + 1; 			//reserve priority_lvl 1
		cur_tcb->cur_prio = i + 1;   			  // cur prio = native prio

		//according to launch_task:
		cur_tcb->context.r4 = (uint32_t) cur_task->lambda;		    //lamba = entry point
		cur_tcb->context.r5 = (uint32_t) cur_task->data;  		    //data = arg
		cur_tcb->context.r6 = (uint32_t) cur_task->stack_pos;     //stack_pos
		cur_tcb->context.r11 = (uint32_t) 0xfeedbeef;

		cur_tcb->holds_lock = 0;
		cur_tcb->sleep_queue = 0;
		//cur->kstack =
		//cur->kstack_high =

		//put it in the run_queue (make it runnable)
		runqueue_add( cur_tcb, cur_tcb->native_prio );
	}

	if(debug_enabled == 1)puts("allocate_tasks...loop complete, doing idle\n");
	//set up idle tcb
	tcb_t* idle_tcb;
	idle_tcb = &system_tcb[OS_MAX_TASKS - 1];     //last TCB, idle task
	//create its tcb entry
	idle_tcb->native_prio = OS_MAX_TASKS - 1;     //reserve priority_lvl 1
	idle_tcb->cur_prio = OS_MAX_TASKS - 1;  			// cur prio = native prio
	//according to launch_task:
	idle_tcb->context.r4 = (uint32_t)&idle;				//lamba = entry point
	idle_tcb->context.r6 = (uint32_t)0;  				  //stack_pos... shouldnt need this
	idle_tcb->context.r11 = (uint32_t)0xfeedbeef;
	idle_tcb->holds_lock = 0;
	idle_tcb->sleep_queue = 0;
	//idle->kstack =
	//idle->kstack_high =
	//put it in the run_queue (make it runnable)
	runqueue_add( idle_tcb, idle_tcb->native_prio );
	if(debug_enabled == 1)puts("allocate_tasks...idle complete\n");
	dispatch_init(idle_tcb);
	if(debug_enabled == 1)puts("allocate_tasks...--\n");
}
