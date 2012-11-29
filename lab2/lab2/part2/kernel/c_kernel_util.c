#include "constants.h"
#include "kernel_util.h"


	/*Updates the contents of ‘vector’ stored at 0x08 to contain LDR pc, [pc, #offset] instruction to cause long branch to address in ‘location’ */
		/*Function returns the original contents of ‘vector’ */

void install_handler(unsigned* return_val, unsigned location, unsigned int *vector)
{
	//unsigned offset;
	//unsigned vec, oldvec;
	//offset = ((unsigned) location – (unsigned) vector – 0x8)
	//	if(offset & 0xFFFFF000) /* check if the offset can be represented using 12 bits */ 
	//	{ 
	//		printf(“Installation of handler failed”);
	//		exit(0); 
	//	}
	//vec =(offset|0xe59FF000);/* vec now contains LDR pc, [pc, #offset] */
	//oldvec = *vector; /* save theveocl=d 0ixne5s9tfrf2u0c0tion at 0x08 */
	//*vector = vec; /* replace the contents of 0x08 with the new LDR instruction */
	//return(oldvec); /* return the old instruction at 0x08 for chaining */ 

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
	unsigned* jump_tab = (unsigned*)(OFFSET_SWI_JUMP + imm);
	unsigned* s_handler = (unsigned*)jump_tab[0];
	
	//Save the first 8 bytes on the stack
	return_val[0] = s_handler[0];
	return_val[1] = s_handler[1];
	return_val[2] = (unsigned)s_handler;
	//Replace them with our instruction and new address
	unsigned our_load = INSTR_OUR_LOAD; // pc = pc - 4
	s_handler[0] = our_load;
	s_handler[1] = location;

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
