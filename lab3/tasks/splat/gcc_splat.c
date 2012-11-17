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
#include <stdlib.h>

int main(int argc, char** argv)
{

	char upright_cursor = '|';
	char fslash_cursor = '/';
	char sideways_cursor = '-';
	char bslash_cursor = '\\';
	char backspace = '\b';
	char space = ' ';
	int flush_screen = 12;
	unsigned long change_state = 200000;
	
	
	while(1)
	{
		printf("%c", (char)flush_screen); 
		printf("%c", space);
		printf("%c", space);
		printf("%c", upright_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);
		printf("%c", space);
		printf("%c", fslash_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);
		printf("%c", space);
		printf("\n");
		printf("%c", space);
		printf("%c", space);	
		printf("%c", space);
		printf("%c", sideways_cursor);
		fflush(stdout);
		usleep(change_state);

		
		printf("%c", backspace);
		printf("%c", space);
		printf("\n");
	        printf("%c", space);
		printf("%c", space);
		printf("%c", space);	
		printf("%c", bslash_cursor);
		fflush(stdout);
		usleep(change_state);

		printf("%c", backspace);
		printf("%c", space);
		printf("%c", backspace);
		printf("%c", backspace);
		printf("%c", upright_cursor);
                fflush(stdout);
                usleep(change_state);

		
		printf("%c", backspace);
		printf("%c", space);
		printf("%c", backspace);
		printf("%c", backspace);
                printf("%c", fslash_cursor);
                fflush(stdout);
                usleep(change_state);


		printf("%c", (char)flush_screen);
                printf("%c", backspace);
		printf("%c", space);
		printf("\n");
                printf("%c", sideways_cursor);
                fflush(stdout);
                usleep(change_state);

		printf("%c", (char)flush_screen);
		printf("%c", space);
                printf("%c", bslash_cursor);
                fflush(stdout);
                usleep(change_state);

		
		


	}

	return 0;
}
