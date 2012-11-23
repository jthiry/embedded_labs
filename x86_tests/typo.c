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
#include <time.h>

int main(int argc, char** argv)
{
  //declare necessary variables
  time_t start_time, end_time;
  char read_buf[60];
  double time_dif = 0.0;
  int err_check = 0;

  while(1) {
    //print out the user prompt
    putc('~', stdout);
    putc(' ', stdout);

    //get the current time
    start_time = time(NULL);

    //printf("start_time: %llu\n", (unsigned long long) start_time);

    //call read to let the user type in the prompt
    //make sure to provide a sensical max length into read
    fgets(read_buf, sizeof(read_buf), stdin);

    //get the current time for comparison
    end_time = time(NULL);

    //printf("end_time: %llu\n", (unsigned long long) end_time);

    //echo what the user typed
    printf("%s\n", read_buf);

    //print how long it took them to type to tenth of second
    time_dif = ((double)end_time - (double)start_time); ///CLOCKS_PER_SEC;
    printf("You took %.1lfs\n", time_dif);
  }

	return 0;
}
