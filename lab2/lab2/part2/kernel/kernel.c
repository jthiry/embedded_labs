/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
	   Josh Thiry <josh.thiry@gmail.com
 * Date:   10/24/2012
 */

#include <exports.h>

void C_SWI_handler(unsigned swi_num, unsigned * regs);

int main(int argc, char *argv[]) {
	
	//Step1: Wire in the SWI Handler
	int* vector_ptr = (int*)0x08;
//	int my_instr = vector_ptr[0];
	//printf( "vector_ptr =%d, vector_ptr *=%d",vector_ptr,*vector_ptr); 
	//void (*function_pointer)(unsigned int, unsigned register*);
	//function_pointer = &C_SWI_handler;
	
		//Verify the vector table is legit
			//Look at the instruction at 0x08
			//confirm it is OK
			// Else output "0x0badc0de"

		//Extract the address of the SWI handler

		//Save the first 8 bytes on the stack

		//Replace them with our instruction and new address

	//Step 2: ...
	
	//Exit Steps: 
		//Restore the 8 bytes from the stack
	return -255;
}
void C_SWI_handler(unsigned int swi_num, unsigned register* regs){
	//Handle Shit
	switch(swi_num){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}

	//Debug
	puts("We are Handling shit\n");
}
