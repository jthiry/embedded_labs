@ kernel_util.S:  contains assembly code to check the vec table instruction,
@			the immediate offset, and enable the user program
@
@ Authors: Joe Battaglia <jabat295@gmail.com>
@          Hans Reichenbach <HansReich25@gmail.com>
@                Josh Thiry <josh.thiry@gmail.com>
@ Date:    10/14/2012
        .file   "kernel_util.S"
        .text
#include "constants.h"



  .global _enable_user_prog
_enable_user_prog:
        STMFD sp!, {r1-r12, lr}   @ sp and lr are banked!!

        mov r3, #START_STACK 	  @ init sp location
        sub r3, r3, #4            @svc sp gets stored here for later
        STR sp, [r3]              @sp we need to save
        sub r3, r3, #4            @link register goes here`for later
        add r2, pc, #12           @figure out where we should link (pc relative)

        str r2, [r3]              @ lr we need to save to return from user prog

        MSR   CPSR, #0x50         @go to user_mode with IRQs ON and FIQs off
        @user mode now!

        mov sp, r0                @ give user sp stack location

        mov pc, r1

        @return here after exit
        @switch back to kernel mode
        @r0 is return status
	mrs r2, cpsr
	orr r2, r2, #0xc0 	@disable irq/fiq
	msr cpsr, r2
        mov r1, #START_STACK
        sub r1, r1, #4
        LDR sp, [r1]
        LDMFD sp!, {r1-r12, lr}
        mov pc, lr

.type _enable_user_prog, function
.size _enable_user_prog, .-_enable_user_prog

 
	.global _check_inst
_check_inst:
        STMFD sp!, {r1-r12, lr}         @store registers and lr
        BIC r0, r0, #0xff               @bit clear to make sure instruction correct
        BIC r0, r0, #0xf00              @bit clear reast of instruction
        LDMFD sp!, {r1-r12, lr}         @load back registers
        mov pc, lr                      @go back to kernel.c

.type _check_inst, function
.size _check_inst, .-_check_inst



        .global _get_imm
_get_imm:
        STMFD sp!, {r1-r12, lr}		@store registers and ls
        ldr r1, =0xfffff000 		@load value to bit clear by since it is too large
        BIC r0, r0, r1			@bit clear to get immediate offset
        LDMFD sp!, {r1-r12, lr}		@load back the registers
        mov pc, lr			@go back to kernel.c

.type _get_imm, function
.size _get_imm, .-_get_imm

