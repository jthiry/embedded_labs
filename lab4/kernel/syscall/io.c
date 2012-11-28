/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>
#include <constants.h>
#include <debug.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

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







/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

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







//returns 1 or 0 based on if buffer is in a valid space
// (true or false)
int not_usable_memory(unsigned loc, unsigned count)
{

	if( (unsigned)loc > START_STACK ||
		(unsigned)loc < END_UBOOT ||
		( (unsigned)loc + count) > START_STACK || ((unsigned)loc + count) < END_UBOOT)
		return 1;
	return 0;

}
