#pragma src
#include "C_SWI_handler.h"

extern void C_SWI_handler(unsigned swi_num, unsigned * regs);

void dummy(void)
{
	unsigned x = 0;
	unsigned* regs;
	regs[0] = 0;
	C_SWI_handler(x, regs);

}
