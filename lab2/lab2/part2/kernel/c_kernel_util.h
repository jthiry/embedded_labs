#ifndef C_KERNEL_UTIL_H
#define C_KERNEL_UTIL_H
void install_handler(unsigned *ret_val, unsigned location, unsigned int *vector);
unsigned* setup_stack(  unsigned stack_start, int argc, char *argv[]);
void uninstall_handler( unsigned* old_instr );
#endif
