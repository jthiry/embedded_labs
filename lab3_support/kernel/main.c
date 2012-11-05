#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include "swi_handler.h"
#include "irq_handler.h"
#include "constants.h"
#include "kernel_util.h"
#include "c_kernel_util.h"

uint32_t global_data;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Wiring in Data abort handler */
	if (-1 == wire_exception_handler(EX_FABRT)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_FABRT));
	    return 0xbadc0de;
	}
	/* setup abort stack */
	setup_abort_stack();
	
/*
	if (-1 == wire_exception_handler(EX_SWI)) {
	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
		    GET_EXP_VEC_ADDR(EX_SWI));
	    return 0xbadc0de;
	}
*/
	/* Wiring in irq handler */
//	if (-1 == wire_exception_handler(EX_IRQ)) {
//	    printf ("ldr pc, [pc, #immed] not encountered at %d. Exiting.\n",
//		    GET_EXP_VEC_ADDR(EX_FABRT));
//	    return 0xbadc0de;
//	}

	//Our code starts here
	
	//Step1: Wire in the SWI Handler
	//Verify the vector table is legit
	//Look at the instruction at 0x08
	//confirm that is of type 'ldr pc, [pc, #imm12]
	//Else output "0x0badc0de"
	unsigned* swi_vec = (unsigned*)0x08;
	unsigned vec_swi = swi_vec[0];

	unsigned inst = _check_inst(vec_swi);
	unsigned imm = _get_imm(vec_swi);
	if(inst != 0xe59ff000)
		return 0x0badc0de;

	//printf("immediate= %x\n", imm);
	//Extract the address of the SWI handler
	//unsigned* jump_tab = (unsigned*)0x24;
	unsigned* jump_tab = (unsigned*)(0x10 + imm);
	unsigned* s_handler = (unsigned*)jump_tab[0];
	//printf("jump_tab=%x jump_tab*=%x\n", jump_tab, *jump_tab);
	//printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);
		//Save the first 8 bytes on the stack
	unsigned word_one = s_handler[0];
	unsigned word_two = s_handler[1];
		//Replace them with our instruction and new address
	unsigned our_load = 0xE51FF004; // pc = pc - 4
	s_handler[0] = our_load;
	s_handler[1] = (unsigned)S_HANDLER;
	puts("Handler Installed...\n");
	//printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);




//	initialize_timer();
	puts("DEBUG::main.c--about to wire in the swi handler\n");
	//Set up the stack
	unsigned* stack_ptr = setup_stack( START_STACK, argc, argv);
	//Start the user program
	int status = _enable_user_prog( (unsigned)stack_ptr, START_USER );
  	puts("DEBUG::main.c--after user prog\n");
	s_handler[0] = word_one;
	s_handler[1] = word_two;
	puts("--Exit steps complete\n");

	return status;
}
