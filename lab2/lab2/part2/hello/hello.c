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
	char* str = "printing out args\n";
	char my_buff[128];		//buffer to hold the input string
	int errs = 0;

  //args testing, input 3 words in command line
	errs = write(1, str, 19);
	printf("args write error: %x\n", errs);
	for(i=0; i < argc; i++)
	{
		errs = write(1, argv[i], 128);
		printf("write loop[%d] error: %x\n", i, errs);
	}

  //check fd checks
	errs = read(7, my_buff, 19);
	printf("fd read error: %x\n", errs);
	errs = write(8, my_buff, 19);
	printf("fd write error: %x\n", errs);

	//check memory bounds
	errs = read(0, (void *)0xf8000000, 7);
	printf("memory ptr high read error: %x\n", errs);
	errs = read(0, (void *)0xa2ffffff, 7);
	printf("memory ptr+size high read error: %x\n", errs);
	errs = write(1, (void *)0xf8000000, 7);
	printf("memory ptr high write error: %x\n", errs);
  errs = write(1, (void *)0xa2ffffff, 7);
	printf("memory ptr+size high write error: %x\n", errs);
	errs = write(1, (void *)0x0, 7);
	printf("memory ptr low write error: %x\n", errs);

	//check generic
	errs = read(0, my_buff, 128);
	printf("read generic error: %x", errs);

	errs = write(1, my_buff, 19);
	printf("write generic error: %x", errs);

	return 0;
}
