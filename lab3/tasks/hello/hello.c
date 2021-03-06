/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdio.h>
//#include <stdlib.h>

int main(int argc, char** argv)
{
	int i, n = 0;
	int loops = 10;
	int millis = 1000;
	if(argc > 2)
	{
		//loops = atoi(argv[1]);
		//millis = atoi(argv[2]);
	}

	const char hello[] = "Sleeping now\r\n";


	printf("Looping %d times for %dms\n", loops, millis);
	printf("Current time is %lu\n",time());
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	for(i = 0; i < loops; i++)
	{
		sleep(millis);
		printf("Loop %d complete, kernel time is %lu ms\n", i, time());
	}
	puts("Sleeping complete\n");
	printf("Current time is %lu\n",time());
	while(n < 100)
	{

		printf("Current time is %lus\n",time());
		n++;

	}

	return 0;
}
