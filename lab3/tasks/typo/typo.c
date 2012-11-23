/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 *
 * @date 11/6/2012
 *
 * Links to libc.
 */

//necessary libs
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_INPUT_LEN 60

int main(int argc, char** argv)
{
	//declare necessary variables
	unsigned long start_time, end_time, time_dif;
	char read_buf[MAX_INPUT_LEN];
	int err_check = 0;
	int dif_ints, dif_dec;

	while(1) {
		//print out the user prompt
		putchar('~');
		putchar(' ');

		//get the current time
		start_time = time();

		//call read to let the user type in the prompt
		//make sure to provide a sensical max length into read
		err_check = read(STDIN_FILENO, read_buf, (size_t) 60);

		//check that read didn't crash
		if(err_check < 1) {
			return -1;
		}

		//get the current time for comparison
		end_time = time();

		//DEBUG
//		printf("start_time: %lu\tend_time: %lu\n", start_time, end_time);

		//echo what the user typed
		write(STDOUT_FILENO, read_buf, err_check);

		//print how long it took them to type to tenth of second
		time_dif = (end_time - start_time);
		dif_ints = time_dif/1000;
		dif_dec = (time_dif/100) - (dif_ints*10);
		printf("You took %d.%d\n", dif_ints, dif_dec);
	}
	return 0;
}
