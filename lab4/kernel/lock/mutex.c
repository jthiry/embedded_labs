/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 *          Hans Reichenbach <HansReich.gmail.com>
 *          Josh Thiry <josh.thiry@gmail.com>
 *
 * @date    11/29/12
 *
 */

#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#include <debug.h>
#endif

int mutexID;
int debug_enabled1 = 1;
int init_flag = 0;
mutex_t gtMutex[OS_NUM_MUTEX];
tcb_t* NULL_TCB = (tcb_t*)0;

void mutex_init()
{
	mutexID = 0;
	if(debug_enabled1 == 1) printf("\tmutex::mutex_init::mutexID = %d\n", mutexID);
	init_flag = 1;
}





int mutex_create(void)
{
	disable_interrupts();
	if(init_flag == 0 ) mutex_init();
	if(mutexID >= OS_NUM_MUTEX ) return -ENOMEM;

	//increment ID count
	mutexID++;

	if(debug_enabled1 == 1) printf("\tmutex::mutex_create new id = %d\n", mutexID);

	//initialize mutex block
	gtMutex[mutexID-1].bAvailable = 	TRUE;
	gtMutex[mutexID-1].bLock = 		FALSE;
	gtMutex[mutexID-1].pHolding_Tcb = 	NULL_TCB;
	gtMutex[mutexID-1].pSleep_queue = 	NULL_TCB;

	enable_interrupts();

	return mutexID;

}

int mutex_lock(int mutex_num)
{
	if(debug_enabled1 == 1) printf("\tmutex::mutex_lock:: lock(%d) by tcb %d\n", mutex_num, get_cur_tcb()->cur_prio);
	
	disable_interrupts();
	
	//check for valid mutex number
	if(mutex_num < 0 || mutex_num > mutexID) return -EINVAL;
	
	//mutex is valid
	mutex_t* mutex = &gtMutex[mutex_num-1]; //mutex we are referencing

	//mutex is available
	if(mutex->bAvailable == TRUE) {
		//acquire  mutex
		mutex->bLock = 		TRUE;
		mutex->bAvailable = 	FALSE;
		mutex->pHolding_Tcb = 	get_cur_tcb();
		get_cur_tcb()->holds_lock++;
		enable_interrupts();
		return 0; //success
	}
	//mutex is not available
	else
	{
		//if current task is already holding it
		if(mutex->pHolding_Tcb == get_cur_tcb()) return -EDEADLOCK;
		
		//if the sleep queue is empty
		if( mutex->pSleep_queue == NULL_TCB ) mutex->pSleep_queue = get_cur_tcb();
		//else sleep queue is not empty
		else
		{
			//add to the end of the queue
			tcb_t* t = mutex->pSleep_queue;
			while(t->sleep_queue != NULL_TCB) t = t->sleep_queue;
			t->sleep_queue = get_cur_tcb();
		}
	}

	//sleep and wait..`
	dispatch_sleep();
	enable_interrupts();
	return 0; //success
}

int mutex_unlock(int mutex_num)
{
	if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock:: id= %d tcb = %d\n", mutex_num, get_cur_tcb()->cur_prio);
	
	disable_interrupts();
	
	//check for valid mutex number
	if(mutex_num < 0 || mutex_num > mutexID) return -EINVAL;
	
	//mutex is valid
	mutex_t* mutex = &gtMutex[mutex_num-1]; //mutex we are referencing
	
	//check if we own this mutex
	if(mutex->pHolding_Tcb != get_cur_tcb()) return -EPERM;

	
	get_cur_tcb()->holds_lock--;
	
	//if the sleep queue is empty
	if(mutex->pSleep_queue == NULL_TCB)
	{
		mutex->bAvailable =	TRUE;
		mutex->bLock = 		FALSE;
		mutex->pHolding_Tcb = 	NULL_TCB;
	}
	//queue not empty
	else
	{
		//next is up
		tcb_t* next = mutex->pSleep_queue;
		
		//mutex held by next
		mutex->pSleep_queue = next->sleep_queue;
		mutex->pHolding_Tcb = next;
		
		//next holds mutex
		next->sleep_queue = NULL_TCB;
		next->holds_lock++;
		
		//next is ready to run
		runqueue_add(next, next->cur_prio);
	}

	enable_interrupts();
	return 0; //success
}
