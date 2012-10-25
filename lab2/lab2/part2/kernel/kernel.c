/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

#include <exports.h>
#include "swi_handler.h"

int main(int argc, char *argv[]) {

	puts("Starting Step 1\n");

	//unsigned* jump_tab, s_handler;
	//unsigned word_one, word_two, our_load;
	//void (*our_swi)();

	//Step1: Wire in the SWI Handler
	//unsigned* swi_vec = (unsigned*)0x08;
	//unsigned vec_swi = swi_vec[0];
		//TODO: Verify the vector table is legit
			//Look at the instruction at 0x08
			//confirm it is OK
			// Else output "0x0badc0de"

		//Extract the address of the SWI handler
	unsigned* jump_tab = (unsigned*)0x24;
	unsigned* s_handler = (unsigned*)jump_tab[0];
	printf("jump_tab=%x jump_tab*=%x\n", jump_tab, *jump_tab);
	printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);


		//Save the first 8 bytes on the stack
	unsigned word_one = s_handler[0];
	unsigned word_two = s_handler[1];

		//Replace them with our instruction and new address
	unsigned our_load = 0xE51FF004; // pc = pc - 4

	s_handler[0] = our_load;
	s_handler[1] = (unsigned)*S_HANDLER;
	puts("Handler Installed...\n");
	printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);

	//Step 2: Switch to user mode with IRQs and FIQs masked`
	puts("Starting Step 2\n");
	printf("Calling ENABLE_USER() at %x\n",&ENABLE_USER);
	//ENABLE_USER();
	puts("User mode enabled, setting up the stack...\n");

	//Step 3: Put user prog args onto the stack
		//first stack location is at 0xa3000000 - 4 = 0xa2FFFFFC
	unsigned* stack_ptr = (unsigned*)0xa3000000;
	//stack_ptr--;
	
	stack_ptr[0] = argc;
	int i;
	for( i = 0; i < argc; i++)
	{	
		printf("Loop %d",i);
		printf("\t++ *stack_ptr= %s = arg[%d] = %s, stack_ptr=%x\n", stack_ptr[i], i, argv[i], stack_ptr);
		stack_ptr--;
		stack_ptr[i+1] = (unsigned)argv[i+1];
	}	
	
	//Exit Steps:
		//Restore the 8 bytes from the stack
	s_handler[0] = word_one;
	s_handler[1] = word_two;
	return -255;
}
