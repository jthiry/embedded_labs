/**
 * @file exception.h
 *
 * @brief Definitions for ARM exception vectoring.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *         Chinmay Kamat <chinmaykamat@cmu.edu>
 *
 * @date Sat, 27 October 2012 02:33:35 EDT
 */

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#define EX_RESET        0
#define EX_UD           1
#define EX_SWI          2
#define EX_FABRT        3
#define EX_DABRT        4
#define EX_IRQ          6
#define EX_FIQ          7
#define NUM_EXCEPTIONS  8

#ifndef ASSEMBLER

#define GET_EXP_VEC_ADDR(exp) ((exp) * 4)
/* C function prototypes go here! Add your group name at the top! */
int  wire_exception_handler(unsigned exception);
void c_abt_handler();
void setup_abort_stack();
void abort_handler(); /* abort_handler for Gravel - defined in abort.S */
void restore_handlers();
#endif /* ASSEMBLER */

#endif /* _EXCEPTION_H_ */
