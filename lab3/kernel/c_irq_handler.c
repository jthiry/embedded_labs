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



int c_irq_handler(unsigned swi_num, unsigned * regs){
	//figure out what caused the interrupt
	//was it the timer interrupt? thats the only one we are servicing
	
	//service the interrupt
	//sleep is done... tell user_prog to continue
	
	//enable IRQs/FIQs
	puts("Inside c_irq_handler.c\n");
	return 0;
}
