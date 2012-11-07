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

	   const char upright_cursor[] = "|";		//upright bar
	   const char fslash_cursor[] = "/";		//forward slanting bar
	   const char sideways_cursor[] = "-";		//sideways bar
	   const char bslash_cursor[] = "\\";		//backward slanting bar
	   const char backspace[] = "\b";		//backspace to erase previous char
	   unsigned long change_state = 200;		//rotate every 200ms
	   int i;

    /*Continous loop that will print out a 
    *rotating bar(cursor). Can't be terminated.*/	  
    while(1)    
	   {
		//print out upright bar, then sleep for 200ms
	   	write(STDOUT_FILENO, upright_cursor, 1);
		sleep(change_state);

		//erase the upright bar, and print out forward slanting bar
		//sleep for 200ms
		write(STDOUT_FILENO, backspace, 1);
		write(STDOUT_FILENO, fslash_cursor, 1);
		sleep(change_state);

		//earase the forward slanting bar, and print out sideways bar
		//sleep for 200ms
		write(STDOUT_FILENO, backspace, 1);
		write(STDOUT_FILENO, sideways_cursor, 1);
		sleep(change_state);

		//erase sideways bar, and print out backward slanting bar
		//sleep 200ms
		write(STDOUT_FILENO, backspace, 1);
		write(STDOUT_FILENO, bslash_cursor, 1);
		sleep(change_state);

		//erase backward slanting bar to start loop again
		write(STDOUT_FILENO,backspace, 1);

	}


	return 0;
}
