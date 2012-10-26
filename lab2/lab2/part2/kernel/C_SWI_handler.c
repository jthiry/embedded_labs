/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */


#include "C_SWI_handler.h"
#include "exit.h"

int C_SWI_handler(unsigned swi_num, unsigned * regs){
  //variables
  int ret = 0;
	//printf("inside C_SWI_HANDLER... swi_num=%d\n",swi_num);

	//Handle Shit
	switch(swi_num){
		case 1:
			//exit
			exit(regs[0]);
			break;
		case 3:
			//puts("++case 3\n");
			//read
			ret = read(regs[0], (void *) regs[1], regs[2]);
			//printf("--case 3, ret= %x\n", ret);
			return ret;

			//check for error and assign to r0 if exists
			break;
		case 4:
			//write
			return write(regs[0], (void *) regs[1], regs[2]);

			//check for error and assign to r0 if exists
			break;

		default:
			//not recognized, throw error
			puts("Invalid syscall recieved\n");
			exit(0x0badc0de);
			break;
	}

	//Debug
	//puts("We are Handling shit\n");

	return ret;
}

//exits the kernel with a given status
void exit(int status) {
  //printf("exit; status=%d\n", status);
  _EXIT(status);
}

//read from a given file into a buffer for count bytes
ssize_t read(int fd, void *buf, size_t count) {
	//convert buf to a char* to make C happy
	char *ourBuf = (char *) buf;
	//puts("reading...\n");
	//printf("fd=%x, *buf=%x, count=%x\n", fd, buf, count);

	//check if fd isn't stdin, return -EBADF if not
	if(fd != STDIN_FILENO) {
		//return error message here
		return EBADF;
	}

	//check if buf loc and size end up outside of useable memory
	if((unsigned)buf > 0xa3000000 || (unsigned)buf < 0x40000000 ||
		((unsigned)buf + count) > 0xa3000000 || ((unsigned)buf + count) < 0x40000000) {
		return EFAULT;
	}
	//puts("reading: passed bounds check\n");

	//read from stdin, we're assuming it's the same as fd
	//loop until buf full
	int bufCount = 0;
	char c;
	while(bufCount < count) {
		//get the next char
		//printf("\t bufCount =%x\n",bufCount);
		c = (char) getc();

		//printf("reading:\t getc=%c\n", c);

			//check for special cases
			if(c == 4) {
				//if char was an EOT

				return bufCount;
			} else if(c == 8 || c == 127) {
				//if char was a backspace or delete

				//remove previous char
				bufCount--;
				ourBuf[bufCount] = '\0';

				//print "\b \b" to sdout
				puts("\b \b");
			} else if(c == 10 || c == 13) {
				//if char was a newline or cr

				//put \n into buffer
				ourBuf[bufCount] = '\n';
				bufCount++;

				//echo \n to stdout
				putc('\n');
				//printf("exiting read, bufCount=%x", bufCount);

				//return bufcount so far
				return bufCount;
			} else {
				//put char into buf and bufCount++
				ourBuf[bufCount] = c;
				bufCount++;

				//output char to stdout
				putc(c);
			}
	}

	//printf("exiting read, bufCount=%x", bufCount);
  //return number of chars read into buffer
  return bufCount;
}

//write a buffer to stdout for count bytes
ssize_t write(int fd, const void *buf, size_t count) {
	//convert the void * into something useful
	char *ourBuf = (char *) buf;

	//printf("writing...\n");

	//check if fd isn't stdout, return -EBADF if not
	if(fd != STDOUT_FILENO) {
		//return error message here
		return EBADF;
	}

	//check if buf loc and size end up outside of useable memory
	if((unsigned)buf > 0xa3000000 || (unsigned)buf < 0x40000000 ||
			((unsigned)buf + count) > 0xa3000000 || ((unsigned)buf + count) < 0x40000000) {
		return EFAULT;
	}

	//printf("made it past the fd and mem checks\n");
	//printf("count = %x\n", count);

	//read from stdout, we're assuming it's the same as fd
	//loop until buf full
	int bufCount = 0;
	unsigned int i = count;
	char c;
	while(i > 0) {
	  //printf("looping... i=%d\n", i);
		//get the next char
		c = (char) ourBuf[bufCount];

		//output char to stdout
		putc(c);

		bufCount++;
		i--;
	}

	//return number of chars read into buffer
	return bufCount;
}
