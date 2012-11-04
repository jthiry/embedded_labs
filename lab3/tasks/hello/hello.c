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

int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";
	char buf[128];
	int read_ret;

	write(STDOUT_FILENO, hello, sizeof(hello) - 1);

	read_ret = read(STDOUT_FILENO, buf, 128);

	write(STDOUT_FILENO, buf, read_ret);

	return 0;
}
