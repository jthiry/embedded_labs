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
#include <debug.h>

unsigned volatile sleeping = 0;

void c_irq_handler(){
	if(debug_enabled==1)puts("c_irq_handler.c::c_irq_handler::++\n");
	//TODO: More defines, move some code over from c_sleep?
	//Which MR?
	unsigned intSrc = reg_read(OSTMR_OSSR_ADDR);
	//MR1 is just a timer update
	if(intSrc & 0x2)
	{	
		reg_write( OSTMR_OSCR_ADDR, 0x0 );            //reset timer
		reg_write( OSTMR_OSMR_ADDR(1), (TIMER_COUNT_PERIOD) );
		reg_write(OSTMR_OSSR_ADDR, 0xFFFFFFFF);       //clear match flag
		/****increment kernel time****/
		kernel_time += TIMER_COUNT_INC;
		if(debug_enabled==1)printf("c_irq_handler.c::c_irq_handler::kernel_time=%lu\n", kernel_time);
	}
	//MR0 is a sleep ending
	else if(intSrc & 0x1)
	{
		if(debug_enabled==1)puts("c_irq_handler.c::c_irq_handler::sleeping=0");
		//stop sleeping
		sleeping = 0;
		reg_write(OSTMR_OSSR_ADDR, 0xFFFFFFFF);       //clear match flag
	}

}
