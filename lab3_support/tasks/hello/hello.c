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
int main(int argc, char** argv)
{
	int i;
	const char hello[] = "Sleeping now\r\n";
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	
	for(i = 0; i < 10; i++)
	{
		sleep(1000);
		printf("kernel time is %lu ms\n",time());	
	}

	return 0;
}
