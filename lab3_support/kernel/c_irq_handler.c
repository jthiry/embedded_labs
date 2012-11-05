/*
 * c_irq_handler.c: This is the C portion of the irq handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   11/03/2012
 */


#include "constants.h"
#include <exports.h>
#include "include/arm/interrupt.h"
#include "include/arm/exception.h"
#include "include/arm/timer.h"
#include "include/arm/psr.h"
#include "include/arm/reg.h"



int c_irq_handler(unsigned irq_num, unsigned * regs){
  //disable IRQs?

	puts("Inside c_irq_handler.c\n");

  //get the time from the counter for drift correction
  uint32_t init_time = reg_read(OSTMR_OSCR_ADDR);

	/* figure out what caused the interrupt
	 * was it the timer interrupt? thats the only one we are servicing*/
	//if(swi_num != INT_OSTMR_0) {
	  //you are not the interrupt we are looking for
	  //return 0;
	//}

	/* service the interrupt
	 * "make note" of the event and reload OSMR0 with appropriate value
	 * account for any drift in time when reloading the register*/

	//figure out the time to reload the match register with
	uint32_t correction = reg_read(OSTMR_OSCR_ADDR) - init_time;

	//add constant for time it takes to execute these instructions?

	//set the match register with the proper value
	reg_write(OSTMR_OSMR_ADDR(0), (TIMER_COUNT_PERIOD - correction));

	//clear the match flag
	reg_write(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

	//increment time count by timer count increment
	kernel_time += TIMER_COUNT_INC;

	//enable IRQs/FIQs?


	return 0;
}
