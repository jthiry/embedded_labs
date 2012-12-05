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

mutex_t gtMutex[OS_NUM_MUTEX];
int mutexID;
int debug_enabled1 = 0;
void mutex_init()
{
  //disable interrupts
  disable_interrupts();

  if(debug_enabled1 == 1) printf("mutex::mutex_init start\n");

	//set up mutex id variable
	mutexID = 0;

	if(debug_enabled1 == 1) printf("mutex::mutex_init::mutexID = %d\n", mutexID);

	//enable interrupts
	enable_interrupts();
}





int mutex_create(void)
{
  //disable interrupts
  disable_interrupts();

  if(debug_enabled1 == 1) printf("mutex::mutex_create start\n");

	//see if a mutex id is available
	if(mutexID >= OS_NUM_MUTEX) {
	  //no mutex IDs available
	  if(debug_enabled1 == 1) printf("mutex::mutex_create no mutexIDs available\n");

	  enable_interrupts();
	  return -ENOMEM;
	}

	//increment ID count
	mutexID++;

	if(debug_enabled1 == 1) printf("mutex::mutex_create new id = %d\n", mutexID);

	//initialize mutex block
	gtMutex[mutexID].bAvailable = TRUE;
	gtMutex[mutexID].bLock = FALSE;
	gtMutex[mutexID].pHolding_Tcb = (tcb_t*)0;
	gtMutex[mutexID].pSleep_queue = (tcb_t*)0;

	if(debug_enabled1 == 1) printf("mutex::mutex_create after initialize mutex\n");

	//enable interrupts
	enable_interrupts();

	return mutexID;

}






int mutex_lock(int mutex)
{
  //disable interrupts
  disable_interrupts();

  if(debug_enabled1 == 1) printf("mutex::mutex_lock start\n");
  if(debug_enabled1 == 1) printf("mutex::mutex_lock requested mutex: %d\n", mutex);
  if(debug_enabled1 == 1) printf("mutex::mutex_lock calling tcb: %x\n", (unsigned int) get_cur_tcb());

  //check validity of the mutex id (return EINVAL if not valid)
  if(mutex < 0 || mutex > mutexID) {
    if(debug_enabled1 == 1) printf("mutex::mutex_lock invalid mutexID\n");

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
  if(!(gtMutex[mutex].bAvailable)) {
    //mutex isn't available so check for deadlock
    if(debug_enabled1 == 1) printf("mutex::mutex_lock mutex not available\n");

    //calling task already owns the mutex
    if(gtMutex[mutex].pHolding_Tcb == get_cur_tcb()) {
      if(debug_enabled1 == 1) printf("mutex::mutex_lock deadlock\n");

      enable_interrupts();
      return -EDEADLOCK;
    }

    //add to sleep queue
    if(gtMutex[mutex].pSleep_queue == (tcb_t *)0) {
      //no sleep queue yet so make this the first

      if(debug_enabled1 == 1) printf("mutex::mutex_lock first sleep queue\n");

      gtMutex[mutex].pSleep_queue = get_cur_tcb();
    } else {
      //a sleep queue already exists so find the end

      if(debug_enabled1 == 1) printf("mutex::mutex_lock add to end of existing sleep queue\n");

      tcb_t* t = gtMutex[mutex].pSleep_queue;
      while(t->sleep_queue > (tcb_t *)-1) {
        t = t->sleep_queue;
      }

      if(debug_enabled1 == 1) printf("mutex::mutex_lock end of sleep queue = %x\n", (unsigned int) t);

      //add the current task to the end of the sleep chain
      t->sleep_queue = get_cur_tcb();
    }

    if(debug_enabled1 == 1) printf("mutex::mutex_lock about to put current tcb to sleep\n");

    //enable interrupts and put task to sleep
    dispatch_sleep();
    enable_interrupts();

    return 0;
  }

  if(debug_enabled1 == 1) printf("mutex::mutex_lock lock the mutex\n");

  //lock the mutex
  gtMutex[mutex].bLock = TRUE;
  gtMutex[mutex].bAvailable = FALSE;
  gtMutex[mutex].pHolding_Tcb = get_cur_tcb();

  //do tcb stuff
  get_cur_tcb()->holds_lock += 1;

  if(debug_enabled1 == 1) printf("mutex::mutex_lock new mutex holding tcb = %x\n", (unsigned int) gtMutex[mutex].pHolding_Tcb);
  if(debug_enabled1 == 1) printf("mutex::mutex_lock cur_tcb holds_lock = %d\n", get_cur_tcb()->holds_lock);

  //enable interrupts
  enable_interrupts();

  //return success
	return 0;
}







int mutex_unlock(int mutex)
{
  //disable interrupts
  disable_interrupts();

  if(debug_enabled1 == 1) printf("mutex::mutex_unlock start\n");
  if(debug_enabled1 == 1) printf("mutex::mutex_unlock requested mutex: %d\n", mutex);
  if(debug_enabled1 == 1) printf("mutex::mutex_unlock calling tcb: %x\n", (unsigned int) get_cur_tcb());

  //check validity of mutex id
  if(mutex < 0 || mutex > mutexID) {
    if(debug_enabled1 == 1) printf("mutex::mutex_unlock invalid mutex\n");

    enable_interrupts();
    return -EINVAL;
  }

  //check if the current task holds the rights to the lock
  if(gtMutex[mutex].pHolding_Tcb != get_cur_tcb()) {
    if(debug_enabled1 == 1) printf("mutex::mutex_unlock tcb doesn't have rights to mutex\n");

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
  if(gtMutex[mutex].pSleep_queue == (tcb_t *) 0) {
    //nothing waiting on mutex so just free it
    if(debug_enabled1 == 1) printf("mutex::mutex_unlock no process waiting on mutex so free it\n");

    gtMutex[mutex].bAvailable = TRUE;
	  gtMutex[mutex].bLock = FALSE;
	  gtMutex[mutex].pHolding_Tcb = (tcb_t*)0;
  } else {
    /*
     * something waiting so shift the queue and tell the waiting
     * task that the mutex is available after passing the mutex
     * to the waiting task
     */

    if(debug_enabled1 == 1) printf("mutex::mutex_unlock something waiting for mutex\n");

    //shift the sleep queue
    tcb_t* t = gtMutex[mutex].pSleep_queue;
    gtMutex[mutex].pSleep_queue = t->sleep_queue;
    t->sleep_queue = (tcb_t *) 0;

    if(debug_enabled1 == 1) printf("mutex::mutex_unlock waiting tcb = %x\n", (unsigned int) t);

    //pass the mutex to the next tcb in the queue
    gtMutex[mutex].pHolding_Tcb = t;
    t->holds_lock += 1;

    if(debug_enabled1 == 1) printf("mutex::mutex_unlock queued tcb holds_lock = %d\n", t->holds_lock);
    if(debug_enabled1 == 1) printf("mutex::mutex_unlock add queued tcb to runqueue\n");

    //activate the sleeping task
    runqueue_add(t, t->cur_prio);
  }

  if(debug_enabled1 == 1) printf("mutex::mutex_unlock mutex handled\n");

  //decrement tcb hold counter
  get_cur_tcb()->holds_lock -= 1;

  if(debug_enabled1 == 1) printf("mutex::mutex_unlock cur_tcb holds_lock = %d\n", get_cur_tcb()->holds_lock);

  //enable interrupts
  enable_interrupts();

	return 0;
}
