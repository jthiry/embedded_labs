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

void c_irq_handler(){
	reg_write( OSTMR_OSCR_ADDR, 0x0 );            //reset timer

	/****deal with match register****/
  //find a way to account for slippage
  reg_write( OSTMR_OSMR_ADDR(0), (TIMER_COUNT_PERIOD-AVG_DRIFT) );
	reg_write(OSTMR_OSSR_ADDR, 0xFFFFFFFF);       //clear match flag

	/****increment kernel time****/
	kernel_time += TIMER_COUNT_INC;

}
