/* @file splat.c:  @brief Displays a spinning cursor.
 *
 *	Authors:
 * 		Joe Battaglia <JABAT295@gmail.com>
 * 		Hans Reichenbach <HansReich25@gmail.com>
 * 		Josh Thiry <josh.thiry@gmail.com>
 *
 * Links to libc.
 */

#include <unistd.h>

int main(int argc, char** argv)
{

	   const char upright_cursor[] = "|";
	   const char fslash_cursor[] = "/";
	   const char sideways_cursor[] = "-";
	   const char bslash_cursor[] = "\\";
	   const char backspace[] = "\b";
	   unsigned long change_state = 200;

//	   while(1)
    int i;
    for(i = 0; i < 20; i++)
	   {
	   	write(STDOUT_FILENO, upright_cursor, 1);
		sleep(change_state);

		write(STDOUT_FILENO, backspace, 1);
		write(STDOUT_FILENO, fslash_cursor, 1);
		sleep(change_state);

		write(STDOUT_FILENO, backspace, 1);
		write(STDOUT_FILENO, sideways_cursor, 1);
		sleep(change_state);

		write(STDOUT_FILENO, backspace, 1);
		write(STDOUT_FILENO, bslash_cursor, 1);
		sleep(change_state);

		write(STDOUT_FILENO,backspace, 1);

	}


	return 0;
}
