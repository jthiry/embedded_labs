/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

#include <exports.h>
#include "s_handler.h"
extern S_HANDLER

void C_SWI_HANDLER(unsigned swi_num, unsigned * regs);

int main(int argc, char *argv[]) {

	puts("Starting Step 1\n");

	//unsigned* jump_tab, s_handler;
	//unsigned word_one, word_two, our_load;
	void (*our_swi)(unsigned int, unsigned register*);

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
	our_swi = &S_HANDLER;
	
	s_handler[0] = our_load;
	s_handler[1] = (unsigned)*our_swi;

	puts("Handler Installed...\n");

	printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);

	//Step 2: ...
	puts("Starting Step 2\n");





	//Exit Steps:
		//Restore the 8 bytes from the stack
	s_handler[0] = word_one;
	s_handler[1] = word_two;
	return -255;
}
