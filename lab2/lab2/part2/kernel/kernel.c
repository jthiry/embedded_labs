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

	//Step1: Wire in the SWI Handler
	//Verify the vector table is legit
	//Look at the instruction at 0x08
	//confirm that is of type 'ldr pc, [pc, #imm12]
	//Else output "0x0badc0de"
	unsigned* swi_vec = (unsigned*)0x08;
	unsigned vec_swi = swi_vec[0];

	unsigned inst = CHECK_INST(vec_swi);	
	unsigned imm = GET_IMM(vec_swi);
	if(inst != 0xe59ff000)
		return 0x0badc0de;
	
	printf("immediate= %x\n", imm);	
	//Extract the address of the SWI handler
	//unsigned* jump_tab = (unsigned*)0x24;
	unsigned* jump_tab = (unsigned*)(0x08 + imm);
	unsigned* s_handler = (unsigned*)jump_tab[0];
	printf("jump_tab=%x jump_tab*=%x\n", jump_tab, *jump_tab);
	printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);
		//Save the first 8 bytes on the stack
	unsigned word_one = s_handler[0];
	unsigned word_two = s_handler[1];
		//Replace them with our instruction and new address
	unsigned our_load = 0xE51FF004; // pc = pc - 4
	s_handler[0] = our_load;
	s_handler[1] = (unsigned)S_HANDLER;
	puts("Handler Installed...\n");
	printf("s_handler=%x s_handler*=%x s_handler**=%x\n", s_handler, *s_handler, s_handler[1]);



	//Step 2: Put user prog args onto the stack
	puts("Starting Step 2\n");
	puts("Setting up the stack...\n");
		//first stack location is at 0xa3000000 - 4 = 0xa2FFFFFC
	unsigned* stack_ptr = (unsigned*)0xa3000000;
	--stack_ptr; // for the svc sp
	--stack_ptr; // for user app LR
	--stack_ptr; // for argc
	*stack_ptr = argc;
	int i;
	for(i = 0; i < argc; i++)
	{
		--stack_ptr;
		*stack_ptr = (unsigned)((char*)argv[i]);
		puts((char*)argv[i]);
		puts("\n");
	}

	//Step 3: Switch to user mode with IRQs and FIQs masked, jump to user program at oxa2000000
	printf("Calling ENABLE_USER_PROG() at %x\n",&ENABLE_USER_PROG);

	int status = ENABLE_USER_PROG((unsigned)stack_ptr);


	//Exit Steps:
	puts("++Starting exit steps");
		//Restore the 8 bytes from the stack
	s_handler[0] = word_one;
	s_handler[1] = word_two;
	puts("--Exit steps complete");
	return status;
}
