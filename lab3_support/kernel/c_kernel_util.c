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
	//TODO: More defined constants
	if(debug_enabled==1)
		puts("c_kernel_util::initilize_timer::++...\n");
	
	//Enable match register 1 and 0 to throw interrupts
	reg_write( INT_ICMR_ADDR, 0x0C000000 );

	// all interrupts are IRQs
	reg_write( INT_ICLR_ADDR, 0x00000000 );
	
	// Match register 1 interrupts every count_period
	reg_write( OSTMR_OSMR_ADDR(1), 16250 );
	
	// OS Count = 0
	reg_write( OSTMR_OSCR_ADDR, 0x0 ); //reset timer
	
	// Match register sets OSSR flag
	reg_write( OSTMR_OIER_ADDR, OSTMR_OSSR_M1); //just MR1 to set flag
	
	//OSSR = clear all interrupt flags
	reg_write( OSTMR_OSSR_ADDR, CLEAR_ALL_FLAGS);

}
void uninitialize_timer()
{
	if(debug_enabled==1)
		puts("c_kernel_util::uninitilize_timer::--...\n");
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
