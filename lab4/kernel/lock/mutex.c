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
	//set up mutex id variable
	mutexID = 0;

	//initialize values for mutex array?
}





int mutex_create(void)
{
  //disable interrupts


	//see if a mutex id is available
	if(mutexID >= OS_NUM_MUTEX) {
	  //no mutex IDs available
	  return -ENOMEM;
	}

	//initialize mutex block

	//assign a new id
	mutexID++;

	//enable interrupts

	return mutexID;

}






int mutex_lock(int mutex  __attribute__((unused)))
{
  //disable interrupts


  //check validity of the mutex id (return EINVAL if not valid)

  //find the right mutex

  /* check if the mutex is available
   *
   * if it isn't then add this task to the queue for it
   * and put the task to sleep
   *
   * also check if this process is the one already holding the
   * mutex and return EDEADLOCK if it is
   */

  //enable interrupts

	return 1; // fix this to return the correct value
}







int mutex_unlock(int mutex  __attribute__((unused)))
{
  //disable interrupts


  //check validity of mutex id

  //check if the current task holds the rights to the lock

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
	return 1; // fix this to return the correct value
}
