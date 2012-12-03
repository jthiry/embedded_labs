/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <debug.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent
 * tasks with logical devices.
 * These logical devices should be signalled periodically
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update
 * on every timer interrupt. In dev_update check if it is
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
  disable_interrupts();
	/* the following line is to get rid of the warning and should not be needed */
	int i;
	for( i=0; i < NUM_DEVICES - 1; i++)
	{
		devices[i].sleep_queue = 0;
		devices[i].next_match = 0;
	}

	enable_interrupts();
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev )
{
  disable_interrupts();

	if(debug_enabled == 1)puts("dev_wait++\n");
	//add to the sleep queue
	tcb_t* sleep_me = get_cur_tcb();
	sleep_me->sleep_queue = devices[dev].sleep_queue;
	devices[dev].sleep_queue = sleep_me;
	if(debug_enabled == 1)printf("dev_wait...dev = %d:::new = (prio) %d:::old = %x\n", dev, (unsigned)devices[dev].sleep_queue->cur_prio, (unsigned)devices[dev].sleep_queue->sleep_queue);

  enable_interrupts();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices.
 * This function should be called on timer interrupts to determine that
 * the interrupt corresponds to the event frequency of a device. If the
 * interrupt corresponded to the interrupt frequency of a device, this
 * function should ensure that the task is made ready to run
 */
void dev_update(unsigned long millis )
{
  disable_interrupts();

	//check each device for an event
	int i;
	int have_some = 0;
	//if(debug_enabled ==1) puts("dev_update::checking \n");
	for( i = 0; i < NUM_DEVICES - 1; i++)
	{
		if( millis >= devices[i].next_match)
		{
			if(debug_enabled ==1) printf("dev_update::waking up %d\n",i);
			//wake up device!
			//make tasks ready to run

			//for each sleeper
			tcb_t* sleepy = devices[i].sleep_queue;
			tcb_t* next;
			while( sleepy != 0)
			{
				//WAKE UP SLEEPY
				runqueue_add(sleepy, sleepy->cur_prio);

				if(debug_enabled == 1)printf("dev_update...dev = %d:::woke up (prio)= %d::next =  %x\n", i, (unsigned)sleepy->cur_prio, (unsigned)sleepy->sleep_queue);
				//another sleeper?
				next = sleepy->sleep_queue;
				sleepy->sleep_queue = 0;
				sleepy = next;

				//set flag that there is a reason to update
				have_some = 1;

			}

			//set next match point in millis
			devices[i].next_match += dev_freq[i];
		}
	}
	//re-evaluate our priorities, if we have a reason to
	if(have_some == 1 ) dispatch_save();

	enable_interrupts();
}
