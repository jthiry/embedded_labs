#include "constants.h"
#include "kernel_util.h"
#include <types.h>
#include "c_swi_handler.h"
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <bits/swi.h>
#include <exports.h>
#include <assert.h>
#include <debug.h>

//let this function see the timer value so it can initialize it
unsigned volatile long kernel_time = 0;

void initialize_timer()
{
	if(debug_enabled==1)puts("c_kernel_util::initilize_timer::++...\n");
	//setup interrupts
	//classify/enable/start
	//OS Timer match registers 0
	//ICMR = 0x04000000 //enabled
	reg_write( INT_ICMR_ADDR, 0x04000000 );

	//ICLR = 0x00000000 // all interrupts are IRQs
	reg_write( INT_ICLR_ADDR, 0x00000000 );
	//OSMR = 10ms => 5ms resolution
	reg_write( OSTMR_OSMR_ADDR(0), TIMER_COUNT_PERIOD );
	//OSCR = 0 to reset timer
	reg_write( OSTMR_OSCR_ADDR, 0x0 ); //reset timer
	//OIER = MR0 enabled
	reg_write( OSTMR_OIER_ADDR, 0x1); //just MR0 enabled
	//OSSR = clear all interrupt flags
	reg_write( OSTMR_OSSR_ADDR, 0xFFFFFFFF );

/*
	//ICLR = 0x04000000 //IRQs
	reg_write( INT_ICLR_ADDR, 0x04000000 );
	//OSMR = 10ms => 5ms resolution
	//reg_write( OSTMR_OSMR_ADDR0, 0x3f7A ); // 5ms = 3.25Mz * .005 = 16250 = 0x3f7A
	reg_write( OSTMR_OSMR_ADDR(0), OSTMR_FREQ ); //JSUT FOR TESTING, MR = 1s
	//OSCR = 0
	reg_write( OSTMR_OSCR_ADDR, 0x0 ); //reset timer
	//OIER = MR0 enabled
	reg_write( OSTMR_OIER_ADDR, OSTMR_OIER_E0 ); //just MR0 enabled

	*/
}
void uninitialize_timer()
{
	if(debug_enabled==1)puts("c_kernel_util::uninitilize_timer::--...\n");
	reg_write( INT_ICMR_ADDR, 0x00000000 );
	reg_write( OSTMR_OIER_ADDR, 0x0); //just MR0 enabled
}

unsigned* setup_stack(  unsigned stack_start, int argc, char *argv[])
{
	//Step 2: Put user prog args onto the stack
	//first stack location is at 0xa3000000 - 4 = 0xa2FFFFFC
	unsigned *stack_ptr = (unsigned*)stack_start;
	--stack_ptr; // for the svc sp
	--stack_ptr; // for user app LR
	int j;
	for(j = argc-1; j >= 0;j--)
	{
		--stack_ptr;
		stack_ptr[0] = (unsigned)argv[j];
	}
	--stack_ptr; // for argc
	stack_ptr[0] = argc;
	return stack_ptr;
}
void uninstall_handler( unsigned* old_instr )
{
	unsigned * s_handler = (unsigned*)old_instr[2];
	s_handler[0] = old_instr[0];
	s_handler[1] = old_instr[1];

}
