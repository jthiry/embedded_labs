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
	//Which MR?
	unsigned intSrc = reg_read(OSTMR_OSSR_ADDR);
	if(debug_enabled==1)printf("IRQ recieved, kernel_time=%lu\n intSrc=%08x\n", kernel_time, intSrc);

	//MR0 is a sleep ending
	if(intSrc & OSTMR_OSSR_M0) 
	{
		sleeping = 0;

	}
	//(We just interrupted a busy wait)
	
	//export time
	kernel_time += (unsigned long)(reg_read(OSTMR_OSCR_ADDR) / CLOCKS_PER_MILLI);
	//Timer=0
	reg_write( OSTMR_OSCR_ADDR, 0x0 );            //reset timer
	//All interupts have been handled
	reg_write(OSTMR_OSSR_ADDR, CLEAR_ALL_FLAGS);       //clear match flag
}
