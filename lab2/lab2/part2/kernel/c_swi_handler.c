/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */


#include "c_swi_handler.h"
#include "exit.h"
#include "constants.h"

int C_SWI_handler(unsigned swi_num, unsigned * regs){
	
	switch(swi_num){
		
		case SWI_NUM_EXIT:
			exit(regs[0]);
			break;

		case SWI_NUM_READ:
			return read(regs[0], (void *) regs[1], regs[2]);

		case SWI_NUM_WRITE:
			return write(regs[0], (void *) regs[1], regs[2]);

		default:
			puts("Invalid syscall recieved\n");
			exit(RET_BAD_CODE);
			break;
	}
	return 0;
}

//exits the kernel with a given status
void exit(int status) { _EXIT(status);}


//read from a given file into a buffer for count bytes
ssize_t read(int fd, void *buf, size_t count) {
	//convert buf to a char* to make C happy
	char *ourBuf = (char *) buf;

	//check if fd isn't stdin, return -EBADF if not
	if(fd != STDIN_FILENO) return -EBADF;

	//check if buf loc and size end up outside of useable memory
	if(not_usable_memory((unsigned)ourBuf) == 1 ) return -EFAULT;

	//read from stdin, we're assuming it's the same as fd
	//loop until buf full
	int bufCount = 0;
	char c;
	while(bufCount < count) {
		//get the next char
		c = (char) getc();
		switch(c)
		{
			case EOT:
				return bufCount; 

			case BACKSPACE:
				//remove previous char
				bufCount--;
				ourBuf[bufCount] = '\0';
				puts("\b \b");
				break;
			
			case DELETE:
				//remove previous char
				bufCount--;
				ourBuf[bufCount] = '\0';
				puts("\b \b");
				break;
			
			case NEWLINE:
				//end input
				ourBuf[bufCount] = '\n';
				bufCount++;
				putc('\n');
				return bufCount;
			
			case CARRIAGE_RETURN:
				ourBuf[bufCount] = '\n';
				bufCount++;
				putc('\n');
				return bufCount;
			
			default:
				//valid character, add it to the buffer
				ourBuf[bufCount] = c;
				bufCount++;
				putc(c);
		}
		return bufCount;
}

//write a buffer to stdout for count bytes
ssize_t write(int fd, const void *buf, size_t count) {
	
	//turn to char* to make C happy
	char *ourBuf = (char *) buf;

	//check if fd isn't stdout, return -EBADF if not
	if(fd != STDOUT_FILENO) return -EBADF;

	//check if buf loc and size end up outside of useable memory
	if(not_usable_memory((unsigned)ourBuf) == 1 ) return -EFAULT;

	//loop until buf full
	int bufCount = 0;
	while(count > 0) {
		//output char to stdout
		putc((char)ourBuf[bufCount]);
		bufCount++;
		count--;
	}

	//return number of chars read into buffer
	return bufCount;
}

//returns 1 or 0
// (true or false)
int not_usable_memory(unsigned loc)
{

	if( (unsigned)loc > 0xa3000000 || 
		(unsigned)loc < 0x40000000 ||
		( (unsigned)loc + count) > 0xa3000000 || ((unsigned)loc + count) < 0x40000000) 
		return 1;
	else 
		return 0;

}
