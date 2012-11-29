/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 *
 * @date
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
int mutexID;

void mutex_init()
{
  //disable interrupts
  disable_interrupts();

	//set up mutex id variable
	mutexID = 0;

	//initialize values for mutex array?

	//enable interrupts
	enable_interrupts();
}





int mutex_create(void)
{
  //disable interrupts
  disable_interrupts();

	//see if a mutex id is available
	if(mutexID >= OS_NUM_MUTEX) {
	  //no mutex IDs available

	  enable_interrupts();
	  return -ENOMEM;
	}

	//increment ID count
	mutexID++;

	//initialize mutex block
	gtMutex[mutexID].bAvailable = TRUE;
	gtMutex[mutexID].bLock = FALSE;
	gtMutex[mutexID].pHolding_Tcb = (tcb_t*)-1;
	gtMutex[mutexID].pSleep_queue = (tcb_t*)-1;

	//enable interrupts
	enable_interrupts();

	return mutexID;

}






int mutex_lock(int mutex  __attribute__((unused)))
{
  //disable interrupts
  disable_interrupts();

  //check validity of the mutex id (return EINVAL if not valid)
  if(mutex < 0 || mutex > mutexID) {
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
    if(gtMutex[mutex].pHolding_Tcb == get_cur_tcb()) {
      enable_interrupts();
      return -EDEADLOCK;
    }

    //add to sleep queue
    if(gtMutex[mutex].pSleep_queue < (tcb_t *)0) {
      //no sleep queue yet so make this the first
      gtMutex[mutex].pSleep_queue = get_cur_tcb();
    } else {
      //a sleep queue already exists so find the end
      tcb_t* t = gtMutex[mutex].pSleep_queue;
      while(t.sleep_queue > (tcb_t *)-1) {
        t = t.sleep_queue;
      }

      //add the current task to the end of the sleep chain
      t.sleep_queue = get_cur_tcb();
    }

    //enable interrupts and put task to sleep
    enable_interrupts();
    dispatch_sleep();
    disable_interrupts();
  }

  //lock the mutex
  gtMutex[mutex].bLock = TRUE;
  gtMutex[mutex].bAvailable = FALSE;
  gtMutex[mutex].pHolding_Tcb = get_cur_tcb();

  //enable interrupts
  enable_interrupts();

  //return success
	return 0;
}







int mutex_unlock(int mutex  __attribute__((unused)))
{
  //disable interrupts
  disable_interrupts();

  //check validity of mutex id
  if(mutex < 0 || mutex > mutexID) {
    enable_interrupts();
    return -EINVAL;
  }

  //check if the current task holds the rights to the lock
  if(gtMutex[mutex].pHolding_Tcb != get_cur_tcb()) {
    enable_interrupts();
    return -EPERM;
  }

  /* check if the mutex has waiting processes
   *
   * if there is a process waiting then remove it from the queue,
   * set it to be the active tcb for the mutex, and let it know
   * it can run now (available flag?)
   *
   * if there isn't a process waiting then clear the holding tcb,
   * set the flag to available, and remove the lock
   */

  //enable interrupts
  enable_interrupts();

	return 1; // fix this to return the correct value
}
