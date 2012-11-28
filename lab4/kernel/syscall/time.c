/** @file time.c
 *
 * @brief Kernel timer based syscall implementations
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 * @author Hans Reichenbach <HansReich.gmail.com>
 * @author Josh Thiry <josh.thiry@gmail.com>
 * @date   11/28/2012
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>


/*
 * @brief Returns the time in millis since the kernel started
 */
unsigned long time_syscall(void)
{
  return kernel_time;
}






/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 *
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
  unsigned long cur_time, stop_time;

	//read time
	cur_time = kernel_time;

	stop_time = cur_time + millis;


	while(cur_time < stop_time )
	{
		//read time
		cur_time = kernel_time;
		//if(debug_enabled == 1)printf("sleeping... cur=%lu < stop=%lu\n", cur_time, stop_time);
	}
}
