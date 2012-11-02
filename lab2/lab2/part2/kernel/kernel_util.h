#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H


extern int ENABLE_USER_PROG(unsigned stack_ptr);	//function to enavle the user program
extern unsigned CHECK_INST(unsigned vec_swi);		//function to check vec table instruction is correct
extern unsigned GET_IMM(unsigned vec_swi);		//function to get the immediate value

#endif


