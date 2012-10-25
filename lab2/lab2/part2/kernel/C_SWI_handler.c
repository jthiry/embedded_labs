/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */


#include "C_SWI_handler.h"

void C_SWI_handler(unsigned swi_num, unsigned * regs){
  //variables
  int err;

	//Handle Shit
	switch(swi_num){
		case 1:
			//exit
			exit(regs[0]);
			break;
		case 3:
		  //read
		  err = read(regs[0], (void *) regs[1], regs[2]);

		  //check for error and assign to r0 if exists
			break;
	  case 4:
	    //write
	    err = write(regs[0], (void *) regs[1], regs[2]);

	    //check for error and assign to r0 if exists
	    break;

		default:
			//not recognized, throw error
      puts("Invalid syscall recieved\n");
			exit(0x0badc0de);
			break;
	}

	//Debug
	puts("We are Handling shit\n");
}

//exits the kernel with a given status
void exit(int status) {

}

//read from a given file into a buffer for count bytes
ssize_t read(int fd, void *buf, size_t count) {
  //convert buf to a char* to make C happy
  char *ourBuf = (char *) buf;

  //check if fd isn't stdin, return -EBADF if not
  if(fd != STDIN_FILENO) {
    //return error message here
    return EBADF;
  }

  //check if buf loc and size end up outside of useable memory
  /* theoretical option 1*/
  if((int)buf > 0xa3000000 || (int)buf < 0x40000000 || ((int)buf + count) > 0xa3000000 || ((int)buf + count) < 0x40000000) {
    return EFAULT;
  }
  /* theoretical option 2
  void *test = malloc(count);
  int compare = (int) test;
  if(compare == 0) {
    return EFAULT;
  }
  free(test);*/

  //read from stdin, we're assuming it's the same as fd
  //loop until buf full
  int bufCount = 0;
  char c;
  while(bufCount < count) {
    //get the next char
    c = (char) getc();

    //check for special cases
    if(c == 4) {
      //if char was an EOT
      return bufCount;
    } else if(c == 8 || c == 127) {
      //if char was a backspace or delete

      //remove previous char
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

  //return number of chars read into buffer
  return bufCount;
}

//write a buffer to stdout for count bytes
ssize_t write(int fd, const void *buf, size_t count) {
  //convert the void * into something useful
  char *ourBuf = (char *) buf;

  //check if fd isn't stdout, return -EBADF if not
  if(fd != STDOUT_FILENO) {
    //return error message here
    return EBADF;
  }

  //check if buf loc and size end up outside of useable memory
  /* TODO */
  /* theoretical option 1*/
  if((int)buf > 0xa3000000 || (int)buf < 0x40000000 || ((int)buf + count) > 0xa3000000 || ((int)buf + count) < 0x40000000) {
    return EFAULT;
  }

  //read from stdout, we're assuming it's the same as fd
  //loop until buf full
  int bufCount = 0;
  unsigned int i = count;
  char c;
  while(i > 0) {
    //get the next char
    c = (char) ourBuf[i];

    //output char to stdout
    putc(c);

    bufCount++;
    i--;
  }

  //return number of chars read into buffer
  return bufCount;
}
