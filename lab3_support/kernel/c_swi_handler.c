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

//test file for debugging assembly
void printAss() {
  puts("DEBUG::swi_handler.S--tick\n");
}


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

  //DEBUG
  puts("DEBUG::c_read--beginning of read\n");

	//convert buf to a char* to make C happy
	char *ourBuf = (char *) buf;

	//check if fd isn't stdin, return -EBADF if not
	if(fd != STDIN_FILENO) return -EBADF;

	//check if buf loc and size end up outside of useable memory
	if(not_usable_memory((unsigned)ourBuf, (unsigned)count) == 1 ) return -EFAULT;

	//DEBUG
	puts("DEBUG::c_read--after checks in read\n");

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

				//DEBUG
	      puts("DEBUG::c_read--returning from read\n");

				return bufCount;

			case CARRIAGE_RETURN:
				ourBuf[bufCount] = '\n';
				bufCount++;
				putc('\n');

				//DEBUG
	      puts("DEBUG::c_read--returning from read\n");

				return bufCount;

			default:
				//valid character, add it to the buffer
				ourBuf[bufCount] = c;
				bufCount++;
				putc(c);
		}
	}

	//DEBUG
	puts("DEBUG::c_read--after read loop\n");

	return bufCount;
}

//write a buffer to stdout for count bytes
ssize_t c_write(int fd, const void *buf, size_t count) {

	//turn to char* to make C happy
	char *ourBuf = (char *) buf;

	//check if fd isn't stdout, return -EBADF if not
	if(fd != STDOUT_FILENO) {
	  return -EBADF;
	}

	//check if buf loc and size end up outside of useable memory
	if(not_usable_memory((unsigned)ourBuf, (unsigned)count) == 1 ) {
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

int c_swi_handler(unsigned swi_num, unsigned * regs){

	switch(swi_num){

		case SWI_NUM_EXIT:
			c_exit(regs[0]);
			break;

		case SWI_NUM_READ:
			return c_read(regs[0], (void *) regs[1], regs[2]);

		case SWI_NUM_WRITE:
			return c_write(regs[0], (void *) regs[1], regs[2]);
		case SWI_NUM_TIME:
			puts("TIME syscall recieved\n");
			break;
		case SWI_NUM_SLEEP:
			puts("SLEEP syscall recieved\n");
			break;
		default:
			puts("Invalid syscall recieved\n");
			c_exit(RET_BAD_CODE);
			break;
	}
	return 0;
}

void hexdump(void *buf, size_t len)
{
	size_t i, j;
	char *b = (char *)buf;

	printf(".---------------------------------------------------------------------------.\n");
	for (i = 0; i < len; i += 16) {
		printf("| %08lx      ", i);
		for (j = i; j < i+16; j++) {
			if (j % 4 == 0)
				printf(" ");
			if (j >= len)
				printf("  ");
			else
				printf("%02x", (unsigned char)b[j]);
		}

		printf("       ");
		for (j = i; j < i+16; j++)
			if (j >= len)
				printf(" ");
			else
				printf("%c", isgraph(b[j]) ? b[j] : '.');
		printf(" |\n");
	}
	printf("`---------------------------------------------------------------------------'\n");
}
