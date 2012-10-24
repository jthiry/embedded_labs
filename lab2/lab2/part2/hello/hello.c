/*
 * hello.c: Hello world! with U-Boot Exports API
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sat, 13 Oct 2007 23:51:13 -0400
 */

#include <exports.h>

int main(void) {

	unsigned long vers_num = get_version();
	unsigned char myChar = 'a';
	unsigned long time;
	while(myChar != 'q')
	{
		puts("myChar=");
		putc(myChar);
		puts("\n");
		time = get_timer(0);
		printf("Wating 3s... timer count = %d\n", time);
		udelay(3000000);
		myChar = getc();	
	}
	printf("This is our version number %d\n", vers_num);
	char* myString = malloc( sizeof(char) * 5);
	free(myString);
	puts("Should I reset the CPU?\n");
	return 0;
}
