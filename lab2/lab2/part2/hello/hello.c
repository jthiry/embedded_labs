/*
 * hello.c: testbed file for making sure our kernel works right
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/26/2012
 */

#include <exports.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	int i;
	int len;
	char* str = "printing out args\n";
	char my_buff[128];		//buffer to hold the input string
	int errs = 0;

  //args testing, input 3 words in command line
	errs = write(1, str, 19);
	printf("args write error: %d\n", errs);
	for(i=0; i < argc; i++)
	{
	  len = 0;
	  while(argv[i][len] != '\0') len++;
	  printf("argv[%d] len is %d\n", i, len);

		errs = write(1, argv[i], len);
		printf("argv[%d] write err: %d\n", i, errs);
		if(errs < 0) {
		  exit(1);
		}
	}

  //check fd checks
  printf("\n\n expected: EBADF (9)\n");
	errs = read(7, my_buff, 19);
	printf("fd read error: %d\n", errs);
	printf("\n\n expected: EBADF (9)\n");
	errs = write(8, my_buff, 19);
	printf("fd write error: %d\n", errs);

	//check memory bounds
	printf("\n\n expected: EFAULT (14)\n");
	errs = read(0, (void *)0xf8000000, 7);
	printf("memory ptr high read error: %d\n", errs);
	printf("\n\n expected: EFAULT (14)\n");
	errs = read(0, (void *)0xa2ffffff, 7);
	printf("memory ptr+size high read error: %d\n", errs);
	printf("\n\n expected: EFAULT (14)\n");
	errs = write(1, (void *)0xf8000000, 7);
	printf("memory ptr high write error: %d\n", errs);
	printf("\n\n expected: EFAULT (14)\n");
  errs = write(1, (void *)0xa2ffffff, 7);
	printf("memory ptr+size high write error: %d\n", errs);
	printf("\n\n expected: EFAULT (14)\n");
	errs = write(1, (void *)0x0, 7);
	printf("memory ptr low write error: %d\n", errs);

	//check generic
	printf("\n\n generic test\n");
	errs = read(0, my_buff, 128);
	printf("read generic error: %d\n", errs);

	errs = write(1, my_buff, 19);
	printf("write generic error: %d\n", errs);

	return 0;
}
