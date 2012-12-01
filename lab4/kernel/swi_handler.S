@ S_HANDLER.S: 	contains assembly complement to kernel.c, which uses these functions to
@				intercept swis and then branch to a user program
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@	         Josh Thiry <josh.thiry@gmail.com>
/* Date:    10/14/2012
*
* 	@this swi handler stores the registers, extracts the swi number, and then passes it to our C_swi_handler
* 	@which handles it. Once the swi is handles we branch back to the user program
*	@author:	Joe Battaglia 		<JABAT295@gmail.com>
*			Hans Reichenbach	<HansReich25@gmail.com>
*			Josh Thiry		<josh.thiry@gmail.com>
*	@date: 11-29-2012
*/

	.file   "swi_handler.S"

#include <asm.h>

FUNC(S_HANDLER)
	/*WE ARE NOW IN SVC MODE*/
	/*we have the SVC sp, and our LR points to the libc wrapper*/

	/* change from supervisor stack to this task's kernel stack*/
	/* save the registers*/
	STMFD sp!, {r0-r11,lr}
	/* find the task's kernel stack*/
	BL      get_cur_tcb
	MOV     r12, r0
	LDMFD   sp!, {r0-r11,lr}
	/* save the old sp then change the sp to the kp*/
	STMFD   r12!, {sp}
	MOV     sp, r12

	/* save the registers to the kernel stack*/
	STMFD sp!, {r0-r11,lr}

	/* save spsr */
  ldr     ip, =global_data2
	mrs     r5, spsr
  str     r5, [ip]

	/* Set up r8 correctly with u-boot dispatch table. */
  ldr      ip, =global_data
  ldr      r8, [ip]

	MRS r0, cpsr
	BIC r0, r0, #0x80	  /*enable IRQs*/
	MSR cpsr, r0


	MOV r1, sp		  /*pass the stack to the handler as an arg*/
	LDR r0, [lr, #-4]         /*load SWI into r0*/
	BIC r0,r0, #0xffffff00	  /*get SWI # by bit-masking*/
	BL c_swi_handler

	/* restore spsr */
  ldr     ip, =global_data2
  ldr     r5, [ip]
  msr     spsr, r5

	str r0, [sp]
	LDMFD sp!, {r0-r11, lr}   /*unstack user's registers*/
	LDMFD sp!, {r12}          /* get the old sp back*/
	MOV   sp, r12             /* switch stack pointers*/
	MOVS  pc, lr              /*return from handler back to the user program*/
	/*THE LAST INSTRUCTION SHOULD HAVE PUT US BACK INTO USER MODE*/
.type S_HANDLER, function
.size S_HANDLER, .-S_HANDLER
