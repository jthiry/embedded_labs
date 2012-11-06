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

#include "typo.h"

int main(int argc, char** argv)
{
  //declare necessary variables
  unsigned long start_time, end_time;
  char read_buf[60];
  float time_dif = 0.0;
  int err_check = 0;

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
    if(err_check < 0) {
      return -1;
    }

    //get the current time for comparison
    end_time = time();

    //echo what the user typed
    printf("%s\n", read_buf);

    //print how long it took them to type to tenth of second
    time_dif = (end_time - start_time)*.001;
    printf("You took %.1fs\n", time_dif);
  }

	return 0;
}
