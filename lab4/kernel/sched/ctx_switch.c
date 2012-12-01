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
#include <arm/exception.h>
#include <debug.h>

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
void dispatch_init(tcb_t* idle )
{
	cur_tcb = idle;
	if(debug_enabled == 1) puts("dispatch_init::+-\n");
}

void ctx_dump(sched_context_t* dmp_ctx, unsigned prio)
{
	printf("Context Dump: %x, prio= %d\n", (unsigned)dmp_ctx, prio);
	printf("\tr4 = %x\n",(unsigned)dmp_ctx->r4);
	printf("\tr5 = %x\n",(unsigned)dmp_ctx->r5);
	printf("\tr6 = %x\n",(unsigned)dmp_ctx->r6);
	printf("\tr7 = %x\n",(unsigned)dmp_ctx->r7);
	printf("\tr8 = %x\n",(unsigned)dmp_ctx->r8);
	printf("\tr9 = %x\n",(unsigned)dmp_ctx->r9);
	printf("\tr10 = %x\n",(unsigned)dmp_ctx->r10);
	printf("\tr11 = %x\n",(unsigned)dmp_ctx->r11);
	printf("\tsp = %x\n",(unsigned)dmp_ctx->sp);
	printf("\tlr = %x\n",(unsigned)dmp_ctx->lr);
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
	//make sure this isn't interrupted
	disable_interrupts();
	if(debug_enabled == 1) puts("\tdispatch_save::++\n");
	if(debug_enabled == 1) printf("\tdispatch_save::cur_tcb->prio = %d\n", cur_tcb->cur_prio);
	if(debug_enabled == 1) puts("\tCurrent");
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);

	//save soon to be previous state for ctx switch
	tcb_t* prev_tcb = cur_tcb;

	//add it back to runnable
	runqueue_add(prev_tcb, prev_tcb->cur_prio);

	//get the next task to run
	uint8_t h_prio = highest_prio();		        //get highest priority
	cur_tcb = runqueue_remove(h_prio);		      //retrieve task while removing it from the run_queue
	if(debug_enabled == 1) printf("\tdispatch_save::after remove::cur_tcb->prio = %d\n", cur_tcb->cur_prio);

	if(debug_enabled == 1) puts("\tDestination");
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);
	//switch tasks
	ctx_switch_full(&cur_tcb->context, &prev_tcb->context);   			//call the context switch (target, current)
	if(debug_enabled == 1) puts("\tdispatch_save::completed ctx_switch_full\n");


	if(debug_enabled == 1) puts("\tdispatch_save::--\n");
	//re-enable interrupts
	enable_interrupts();
}

/**
 * @brief Context switch to the highest priority task that is not this task --
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	if(debug_enabled == 1) puts("\tdispatch_nosave::++\n");
	//disable interrupts so we don't mess up the ctx switch
	disable_interrupts();
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);

	//get the next task to run
	uint8_t h_prio = highest_prio();	    //get highest priority
	cur_tcb = runqueue_remove(h_prio);	  //retrieve task while removing it from runqueue
	if(debug_enabled == 1) printf("\tdispatch_nosave::after remove::cur_tcb->prio = %d\n", cur_tcb->cur_prio);
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);

	//run the next task
	ctx_switch_half(&cur_tcb->context);		          //call the half context switch

	//we should never return here

	if(debug_enabled == 1) puts("\tdispatch_nosave::completed ctx_switch_half\n");
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);

	//re-enable interrupts
	enable_interrupts();
	if(debug_enabled == 1) puts("\tdispatch_nosave::--\n");
}


/**
 * @brief Context switch to the highest priority task that is not this task --
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	if(debug_enabled == 1) puts("\tdispatch_sleep::++\n");
	//disable interrupts for the ctx switch
	disable_interrupts();
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);

	if(debug_enabled == 1) printf("\tdispatch_sleep::cur_tcb->prio = %d\n", cur_tcb->cur_prio);
	//save the soon to be previous tcb
	tcb_t* prev_tcb = cur_tcb;

	//get the next task to run
	uint8_t h_prio = highest_prio();		      //get highest priority
	cur_tcb = runqueue_remove(h_prio);		    //retrieve task while removing it from the runqueue
	

	///debug block
	if(debug_enabled == 1) printf("\tdispatch_sleep::after remove::cur_tcb->prio = %d\n", cur_tcb->cur_prio);
	if(debug_enabled == 1) puts("\tCurrent ");
	if(debug_enabled == 1) ctx_dump(&prev_tcb->context, prev_tcb->cur_prio);
	if(debug_enabled == 1) puts("\tDestination ");
	if(debug_enabled == 1) ctx_dump(&cur_tcb->context, cur_tcb->cur_prio);
	
	//switch tasks
	ctx_switch_full(&cur_tcb->context, &prev_tcb->context);   		//call the context switch

	//re-enable interrupts
	enable_interrupts();
	if(debug_enabled == 1) puts("\tdispatch_sleep::--\n");
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
