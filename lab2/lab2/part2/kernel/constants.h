#ifndef CONSTANTS_H
#define CONSTANTS_H

#define VECTOR_SWI 0x08

#define RET_BAD_CODE 0x0badc0de

#define INSTR_TYPE_LOAD 0xe59ff000
#define INSTR_OUR_LOAD 0xe51ff004

#define OFFSET_SWI_JUMP 0x10

#define STACK_START 0xa3000000
#define START_LOCATION 0xa2000000

#define EOT 4
#define BACKSPACE 8
#define DELETE 127
#define NEWLINE 10
#define CARRIAGE_RETURN 13

#define SWI_NUM_EXIT 1
#define SWI_NUM_READ 3
#define SWI_NUM_WRITE 4

#endif