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
#include <device.h>
#include <arm/interrupt.h>
#include <arm/exception.h>
#include <arm/timer.h>
#include <arm/psr.h>
#include <arm/reg.h>
#include <debug.h>

unsigned volatile sleeping = 0;

void c_irq_handler(){
	//Which MR?
	unsigned intSrc = reg_read(OSTMR_OSSR_ADDR);

	//MR1 is just a timer update
	if(intSrc & 0x2)
	{
//		if(debug_enabled == 1) puts("c_irq_handler.c::++\n");
		reg_write( OSTMR_OSCR_ADDR, 0x0 );            //reset timer
		reg_write( OSTMR_OSMR_ADDR(1), (TIMER_COUNT_PERIOD) ); //set the match register value
		reg_write(OSTMR_OSSR_ADDR, 0xFFFFFFFF);       //clear match flag

		/****increment kernel time****/
		kernel_time += TIMER_COUNT_INC;
		dev_update(kernel_time);
//		if(debug_enabled == 1) puts("c_irq_handler.c::--\n");
	}

}
