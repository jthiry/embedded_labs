/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */


#include "exit.h"
#include "constants.h"
#include <exports.h>
#include "bits/fileno.h"
#include "bits/errno.h"
#include "include/arm/interrupt.h"
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include "include/arm/timer.h"
#include <debug.h>



//returns 1 or 0
// (true or false)
int not_usable_memory(unsigned loc, unsigned count)
{

	if( (unsigned)loc > START_STACK ||
		(unsigned)loc < END_UBOOT ||
		( (unsigned)loc + count) > START_STACK || ((unsigned)loc + count) < END_UBOOT)
		return 1;
	return 0;

}


//exits the kernel with a given status
void c_exit(int status) { _exit(status);}


//read from a given file into a buffer for count bytes
ssize_t c_read(int fd, void *buf, size_t count) {

	//convert buf to a char* to make C happy
	char *ourBuf = (char *) buf;

	//check if fd isn't stdin, return -EBADF if not
	if(fd != STDIN_FILENO)
	{
		return -EBADF;
	}
	//check if buf loc and size end up outside of useable memory
	if(not_usable_memory((unsigned)ourBuf, (unsigned)count) == 1 )
	{
		return -EFAULT;
	}

	//read from stdin, we're assuming it's the same as fd
	//loop until buf full
	unsigned bufCount = 0;
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
	}

	return bufCount;
}

//write a buffer to stdout for count bytes
ssize_t c_write(int fd, const void *buf, size_t count) {

	//turn to char* to make C happy
	char *ourBuf = (char *) buf;

	//check if fd isn't stdout, return -EBADF if not
	if(fd != STDOUT_FILENO) {
		if(debug_enabled ==1) printf ("c_write:: EBADF\n") ;
		return -EBADF;
	}

	//check if buf loc and size end up outside of useable memory
	if(not_usable_memory((unsigned)ourBuf, (unsigned)count) == 1 ) {
		if(debug_enabled ==1) printf ("c_write:: EFAULT\n") ;
	  return -EFAULT;
	}

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

//check the time since the kernel was loaded
size_t c_time() {
	return kernel_time;
}

//stops execution for a given period of time
void c_sleep(size_t millis) {

	unsigned long cur_time, stop_time;

	//read time
	cur_time = kernel_time;

	stop_time = cur_time + millis;


	while(cur_time < stop_time )
	{
		//read time
		cur_time = kernel_time;
		//if(debug_enabled == 1)printf("sleeping... cur=%lu < stop=%lu\n", cur_time, stop_time);
	}
}

//call the appropriate method based on the swi
int c_swi_handler(unsigned swi_num, unsigned * regs){

	if(debug_enabled ==1) printf ("c_swi_handler:: swi_num = %d\n", swi_num ) ;
	switch(swi_num){

		case SWI_NUM_EXIT:
			c_exit(regs[0]);
			break;

		case SWI_NUM_READ:
			return c_read(regs[0], (void *) regs[1], regs[2]);

		case SWI_NUM_WRITE:
			return c_write(regs[0], (void *) regs[1], regs[2]);

		case SWI_NUM_TIME:
			return c_time();

		case SWI_NUM_SLEEP:
			c_sleep(regs[0]);
			break;

		default:
			puts("Invalid syscall recieved\n");
			c_exit(RET_BAD_CODE);
			break;
	}
	return 0;
}
