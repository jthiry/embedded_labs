/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *Authors:
 *		Joe Battaglia <JABAT295@gmail.com>
 *		Hans Reichenbach <HansReich25@gmail.com>
 *		Josh Thiry <josh.thiry@gmail.com>
 * Links to libc.
 */

#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int upright_cursor = '|';
	int fslash_cursor = '/';
	int sideways_cursor = '-';
	int bslash_cursor = '\\';
	int backspace = '\b';
	unsigned long change_state = 200000;

	while(1)
	{
		printf("%c", (char)upright_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);
		printf("%c", (char)fslash_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);
		printf("%c", (char)sideways_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);
		printf("%c", (char)bslash_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);

	}

	return 0;
}
