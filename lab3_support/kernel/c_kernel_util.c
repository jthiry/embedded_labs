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
//#include <arm/reg.h>
//#define INT_ICIP_ADDR   0x00D00000  /* Interrupt Controller IRQ Pending Register */
//#define INT_ICMR_ADDR   0x00D00004  /* Interrupt Controller Mask Register */
//#define INT_ICLR_ADDR   0x00D00008  /* Interrupt Controller Level Register */
//#define INT_ICFP_ADDR   0x00D0000C  /* Interrupt Controller FIQ Pending Register */
//#define INT_ICPR_ADDR   0x00D00010  /* Interrupt Controller Pending Register */
//#define OSTMR_OSMR_ADDR(x)    (0x00A00000 + (x)*4)    /* OS Timer Match Register */
//#define OSTMR_OSCR_ADDR       0x00A00010   /* OS Timer Count Register */
//#define OSTMR_OIER_ADDR       0x00A0001C   /* OS Timer Interrupt Enable Register */
//#define OSTMR_OIER_E0         0x00000001   /* Enable match 0 */
//#define OSTMR_FREQ            3250000      /* Oscillator frequency in hz */
//
/*
INLINE uint32_t reg_read(size_t addr)
INLINE void reg_write(size_t addr, uint32_t data)
INLINE void reg_set(size_t addr, uint32_t flags)
INLINE void reg_clear(size_t addr, uint32_t flags)

*/

//let this function see the timer value so it can initialize it
unsigned long kernel_time = 0;

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
	//reg_write( OSTMR_OSMR_ADDR0, 0x3f7A ); // 5ms = 3.25Mz * .005 = 16250 = 0x3f7A 
	//CALCULATED DRIFT = 6,500 cycles => 16,250 + 6,500 = 22,750
	reg_write(  0x00A00000, 22750 ); // 5ms = 3.25Mz * .005 = 16250 = 0x3f7A 
	//reg_write(  0x00A00000, 1625000); // 0.5s
	//reg_write( 0x00A00000, 3250000 ); //JSUT FOR TESTING, MR = 1s
	//OSCR = 0
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

/*
void install_handler(unsigned* return_val, unsigned location, unsigned int *vector, unsigned offset)
{
	unsigned* swi_vec = vector;
	unsigned vec_swi = swi_vec[0];


	unsigned inst = _check_inst(vec_swi);
	unsigned imm = _get_imm(vec_swi);
	if(inst != INSTR_TYPE_LOAD)
	{
		return_val[0] = (unsigned)RET_BAD_CODE;
		return ;
	}
	//Extract the address of the SWI handler
	//unsigned* jump_tab = (unsigned*)0x24;
	unsigned* jump_tab = (unsigned*)(offset + imm);
	unsigned* old_handler = (unsigned*)jump_tab[0];


	//Save the first 8 bytes on the stack
	return_val[0] = old_handler[0];
	return_val[1] = old_handler[1];
	return_val[2] = (unsigned)old_handler;
	//Replace them with our instruction and new address
	old_handler[0] = INSTR_OUR_LOAD; // pc = pc - 4
	old_handler[1] = location;

}
*/
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
