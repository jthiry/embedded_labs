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

//create the volatile global for time storage (in milliseconds)

void c_irq_handler(){

	reg_write( OSTMR_OSCR_ADDR, 0x0 ); //reset timer
	reg_write(OSTMR_OSSR_ADDR, 0xFFFFFFFF);
	kernel_time += 5;
}
