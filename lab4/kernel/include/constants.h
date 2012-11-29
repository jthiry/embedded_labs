#ifndef CONSTANTS_H
#define CONSTANTS_H

#define VECTOR_SWI 0x08
#define VECTOR_IRQ 0x0C
#define OFFSET_SWI_JUMP 0x10
#define OFFSET_IRQ_JUMP 0x14

#define RET_BAD_CODE 0x0badc0de

#define INSTR_TYPE_LOAD 0xe59ff000
#define INSTR_OUR_LOAD 0xe51ff004

#define START_KERNEL 0xa3000000

#define SWI_NUM_EXIT 1
#define SWI_NUM_READ 3
#define SWI_NUM_WRITE 4
#define SWI_NUM_TIME 6
#define SWI_NUM_SLEEP 7

#endif
