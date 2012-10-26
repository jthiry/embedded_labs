/*
 * hello.c: Hello world! with U-Boot Exports API
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sat, 13 Oct 2007 23:51:13 -0400
 */

#include <exports.h>

int main(int argc, char* argv[]) {

	int i;
	char* str = "printing out args\n"
	char my_buff[128];		//buffer to hold the input string

	write(1, str, 19);
	for(i=0; i < argc; i++)
	{
		write(1, argv[i], 128);
	}

	read(0, my_buff, 128); 

	write(1, my_buff, 19);
	/*things to check
    	buffer bounds
		buffer+size bounds
		filenumbers


	return 0;
}
