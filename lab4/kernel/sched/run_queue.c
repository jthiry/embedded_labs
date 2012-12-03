/** @file run_queue.c
 *
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"
#include <debug.h>


static tcb_t* run_list[OS_MAX_TASKS];

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8];

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits ;

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
	//clear run_list TODO:

	//clear group_run_bits
	group_run_bits = 0;

	//clear run_bits
	int i;
	for( i = 0; i < OS_MAX_TASKS/8 - 1 ; i++)
	{
		run_bits[i] = 0;
	}
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb, uint8_t prio )
{
	if(debug_enabled == 1)printf("runqueue_add...++, task %d\n", (unsigned)prio);
	//put tcb in run_list
	run_list[prio] = tcb;

	//set appropriate bits to one
	uint8_t y = (prio >> 3); 	//loc in group_bits
	uint8_t x = (prio & 0x07); 	//loc in run_bits
	group_run_bits = (group_run_bits | (1 << y) );
	run_bits[y] = (run_bits[y] | (1 << x));

	//if(debug_enabled == 1)printf("end runqueue_add...--, x = %d:: y = %d:: prio = %d:: group_bits = %x:: run_bits = %x\n", (unsigned)x, (unsigned)y, (unsigned)prio, (unsigned)group_run_bits, (unsigned)run_bits[y]);

}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio)
{
	if(debug_enabled == 1)printf("runqueue_remove...++, task %d\n", (unsigned)prio);
//	if(debug_enabled == 1)printf("runqueue_remove...++, prio = %d\n", (unsigned)prio);
	//put tcb in run_list
	tcb_t* ret_tcb;

	if( prio >= OS_MAX_TASKS - 1 ) return run_list[OS_MAX_TASKS -1];

	//remove tcb from run_list
	ret_tcb = run_list[prio];
	run_list[prio] = 0;

	//set appropriate bits to zero
	uint8_t y = (prio >> 3);
	uint8_t x = (prio & 0x07);
	run_bits[y] = (run_bits[y] & ~(1 << x));
	if(run_bits[y] == 0)
		//only clear the whole group's flag if the whole group is empty
		group_run_bits = (group_run_bits & ~(1 << y) );

	//if(debug_enabled == 1)printf("end runqueue_remove...--, x = %d:: y = %d:: prio = %d:: group_bits = %x:: run_bits = %x\n", (unsigned)x, (unsigned)y, (unsigned)prio, (unsigned)group_run_bits, (unsigned)run_bits[y]);
	return ret_tcb;
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
	uint8_t ret;
	if(group_run_bits == 0) return ( OS_MAX_TASKS - 1); //idle task priority
	uint8_t y = prio_unmap_table[group_run_bits];
	uint8_t x = prio_unmap_table[run_bits[y]];
	ret = (y << 3) + x;
//	if(debug_enabled == 1)printf("highest_prio = %d\n", (unsigned)ret);
	return ret;
}
