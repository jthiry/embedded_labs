/** @file ctx_switch.c
 *
 * @brief C wrappers around assembly context switch routines.
 *
 *  @author  Joe Battaglia <JABAT295.gmail.com>
 *          Hans Reichenbach <HansReich.gmail.com>
 *          Josh Thiry <josh.thiry@gmail.com>
 * @date 2012-11-29
 */


#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{

}


/**
 * @brief Context switch to the highest priority task while saving off the
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	//add it back to runnable
	runqueue_add(cur_tcb, cur_tcb->cur_prio);

	uint8_t h_prio = highest_prio();		    //get highest priority
	tcb_t* temp_tcb = cur_tcb;
	cur_tcb = runqueue_remove(h_prio);		  //retrieve task while removing it from the run_queue
	ctx_switch_full(cur_tcb, temp_tcb);			//call the context switch

}

/**
 * @brief Context switch to the highest priority task that is not this task --
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	uint8_t h_prio = highest_prio();	    //get highest priority
	cur_tcb = runqueue_remove(h_prio);	  //retrieve task while removing it from runqueue
	ctx_switch_half(cur_tcb);		          //call the half context switch

}


/**
 * @brief Context switch to the highest priority task that is not this task --
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	//add the current task to the sleep queue
	//sleepqueue_add(cur_tcb, cur_tcb->cur_prio);

	uint8_t h_prio = highest_prio();		  //get highest priority
	tcb_t* temp_tcb = cur_tcb;            //save the current tcb to switch
	cur_tcb = runqueue_remove(h_prio);		//retrieve task while removing it from the runqueue
	ctx_switch_full(cur_tcb, temp_tcb);		//call the context switch

} //BROKEN AS SHEEIT

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return (*cur_tcb).cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
