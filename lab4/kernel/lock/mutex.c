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
int debug_enabled1 = 0;
int init_flag = 0;
mutex_t gtMutex[OS_NUM_MUTEX];
tcb_t* NULL_TCB = (tcb_t*)0;

// used for priority inheritance
static uint8_t cur_prio_ceiling;

/* handlers for cur_prio_ceiling*/
uint8_t get_cur_prio_ceiling(void) {
	return cur_prio_ceiling;
}

void set_cur_prio_ceiling(uint8_t prio) {
	cur_prio_ceiling = prio;
}



/* make sure mutexes are ready to rock and roll for the programs*/
void mutex_init()
{
	mutexID = 0;
	if(debug_enabled1 == 1) printf("\tmutex::mutex_init::mutexID = %d\n", mutexID);
	init_flag = 1;
	cur_prio_ceiling = IDLE_PRIO;
}




/**
 * creates a mutex if there are still mutexIDs available
 *
 * returns ENOMEM if there is no space left for new mutexes
 *
 * returns the mutex ID on success
 **/
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
	gtMutex[mutexID-1].bLock = 			FALSE;
	gtMutex[mutexID-1].pHolding_Tcb = 	NULL_TCB;
	gtMutex[mutexID-1].pSleep_queue = 	NULL_TCB;
	gtMutex[mutexID-1].ceiling_prio =	IDLE_PRIO;

	enable_interrupts();

	return mutexID;

}








/**
 * check if the indicated mutex is available to lock
 *
 * if the mutex is unavailable then puts the task to sleep until it's ready
 *
 * returns errors if invalid mutex given or if the requesting task already controls the mutex
 *
 * returns 0 on success
 **/
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







/**
 * unlocks a given mutex if the requesting task controls it
 *
 * if another task is waiting on the mutex runs it
 *
 * returns errors if invalid mutex given or if the requesting task doesn't control the mutex
 *
 * returns 0 on success
 **/
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
