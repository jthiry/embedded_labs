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

void mutex_init()
{

	//set up mutex id variable
	mutexID = 0;

	if(debug_enabled1 == 1) printf("\tmutex::mutex_init::mutexID = %d\n", mutexID);

}





int mutex_create(void)
{
	//disable interrupts
	disable_interrupts();
	if(init_flag == 0 )
	{
		mutex_init();
		init_flag = 1;

	}

	//if(debug_enabled1 == 1) printf("\tmutex::mutex_create start\n");

	//see if a mutex id is available
	if(mutexID >= OS_NUM_MUTEX ) {
		//no mutex IDs available
		if(debug_enabled1 == 1) printf("\tmutex::mutex_create no mutexIDs available\n");

		enable_interrupts();
		return -ENOMEM;
	}

	//increment ID count
	mutexID++;

	if(debug_enabled1 == 1) printf("\tmutex::mutex_create new id = %d\n", mutexID);

	//initialize mutex block
	gtMutex[mutexID-1].bAvailable = TRUE;
	gtMutex[mutexID-1].bLock = FALSE;
	gtMutex[mutexID-1].pHolding_Tcb = (tcb_t*)0;
	gtMutex[mutexID-1].pSleep_queue = (tcb_t*)0;

	//if(debug_enabled1 == 1) printf("\tmutex::mutex_create after initialize mutex\n");

	//enable interrupts
	enable_interrupts();

	return mutexID;

}






int mutex_lock(int mutex)
{
	//disable interrupts
	disable_interrupts();

	if(debug_enabled1 == 1) printf("\tmutex::mutex_lock:: id= %d tcb = %d\n", mutex, get_cur_tcb()->cur_prio);

	//check validity of the mutex id (return EINVAL if not valid)
	if(mutex < 0 || mutex > mutexID) {
		if(debug_enabled1 == 1) printf("\tmutex::mutex_lock invalid mutex = %d and mutexID=%d\n", mutex, mutexID );

		enable_interrupts();
		return -EINVAL;
	}

	/* check if the mutex is available
	 *
	 * if it isn't then add this task to the queue for it
	 * and put the task to sleep
	 *
	 * also check if this process is the one already holding the
	 * mutex and return EDEADLOCK if it is
	 */
	if(!(gtMutex[mutex-1].bAvailable)) {
		//mutex isn't available so check for deadlock
		if(debug_enabled1 == 1) printf("\tmutex::mutex_lock mutex not available\n");

		//calling task already owns the mutex
		if(gtMutex[mutex-1].pHolding_Tcb == get_cur_tcb()) {
			if(debug_enabled1 == 1) printf("\tmutex::mutex_lock deadlock\n");

			enable_interrupts();
			return -EDEADLOCK;
		}

		//add to sleep queue
		if(gtMutex[mutex-1].pSleep_queue == (tcb_t *)0) {
			//no sleep queue yet so make this the first

			if(debug_enabled1 == 1) printf("\tmutex::mutex_lock first sleep queue\n");

			gtMutex[mutex-1].pSleep_queue = get_cur_tcb();
		} else {
			//a sleep queue already exists so find the end

			if(debug_enabled1 == 1) printf("\tmutex::mutex_lock add to end of existing sleep queue\n");

			tcb_t* t = gtMutex[mutex-1].pSleep_queue;
			while(t->sleep_queue > (tcb_t *)-1) {
				t = t->sleep_queue;
			}

			if(debug_enabled1 == 1) printf("\tmutex::mutex_lock end of sleep queue = %x\n", (unsigned int) t);

			//add the current task to the end of the sleep chain
			t->sleep_queue = get_cur_tcb();
		}

		if(debug_enabled1 == 1) printf("\tmutex::mutex_lock about to put current tcb to sleep\n");

		//enable interrupts and put task to sleep
		dispatch_sleep();
		enable_interrupts();

		return 0;
	}

	//if(debug_enabled1 == 1) printf("\tmutex::mutex_lock lock the mutex\n");

	//lock the mutex
	gtMutex[mutex-1].bLock = TRUE;
	gtMutex[mutex-1].bAvailable = FALSE;
	gtMutex[mutex-1].pHolding_Tcb = get_cur_tcb();

	//do tcb stuff
	get_cur_tcb()->holds_lock += 1;

	//if(debug_enabled1 == 1) printf("\tmutex::mutex_lock new mutex holding tcb = %x\n", (unsigned int) gtMutex[mutex-1].pHolding_Tcb);
	//if(debug_enabled1 == 1) printf("\tmutex::mutex_lock cur_tcb holds_lock = %d\n", get_cur_tcb()->holds_lock);

	//enable interrupts
	enable_interrupts();

	//return success
	return 0;
}







int mutex_unlock(int mutex)
{
	//disable interrupts
	disable_interrupts();

	if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock:: id= %d tcb = %d\n", mutex, get_cur_tcb()->cur_prio);

	//check validity of mutex id
	if(mutex < 0 || mutex > mutexID) {
		if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock invalid mutex\n");

		enable_interrupts();
		return -EINVAL;
	}

	//check if the current task holds the rights to the lock
	if(gtMutex[mutex-1].pHolding_Tcb != get_cur_tcb()) {
		if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock tcb doesn't have rights to mutex\n");

		enable_interrupts();
		return -EPERM;
	}

	/* check if the mutex has waiting processes
	 *
	 * if there is a process waiting then remove it from the queue,
	 * set it to be the active tcb for the mutex, and let it know
	 * it can run now
	 *
	 * make sure not to clobber the sleep queue, reset it to the
	 * next one in line
	 *
	 * if there isn't a process waiting then clear the holding tcb,
	 * set the flag to available, and remove the lock
	 */
	if(gtMutex[mutex-1].pSleep_queue == (tcb_t *) 0) {
		//nothing waiting on mutex so just free it
		if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock no process waiting on mutex so free it\n");

		gtMutex[mutex-1].bAvailable = TRUE;
		gtMutex[mutex-1].bLock = FALSE;
		gtMutex[mutex-1].pHolding_Tcb = (tcb_t*)0;
	} else {
		/*
		 * something waiting so shift the queue and tell the waiting
		 * task that the mutex is available after passing the mutex
		 * to the waiting task
		 */

		//if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock something waiting for mutex\n");

		//shift the sleep queue
		tcb_t* t = gtMutex[mutex-1].pSleep_queue;
		gtMutex[mutex-1].pSleep_queue = t->sleep_queue;
		t->sleep_queue = (tcb_t *) 0;

		//if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock waiting tcb = %x\n", (unsigned int) t);

		//pass the mutex to the next tcb in the queue
		gtMutex[mutex-1].pHolding_Tcb = t;
		t->holds_lock += 1;

		if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock queued tcb holds_lock = %d\n", t->holds_lock);
		//if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock add queued tcb to runqueue\n");

		//activate the sleeping task
		runqueue_add(t, t->cur_prio);
	}

	//if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock mutex handled\n");

	//decrement tcb hold counter
	get_cur_tcb()->holds_lock -= 1;

	if(debug_enabled1 == 1) printf("\tmutex::mutex_unlock cur_tcb holds_lock = %d\n", get_cur_tcb()->holds_lock);

	//enable interrupts
	enable_interrupts();

	return 0;
}
