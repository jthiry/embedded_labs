#ifndef KERNEL_UTIL_H
#define KERNEL_UTIL_H


extern int _enable_user_prog(unsigned stack_ptr);	//function to enavle the user program
extern unsigned _check_inst(unsigned vec_swi);		//function to check vec table instruction is correct
extern unsigned _get_imm(unsigned vec_swi);		//function to get the immediate value

#endif


