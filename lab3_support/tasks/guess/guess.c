/** @file guess.c
 *
 * @brief Guessing game where the user tries to guess how long a sleep period was
 *
 * @author Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 *
 * @date 11/8/2012
 *
 * Links to libc.
 */

#define MAX_SLEEP         15000   //max length of sleep in milliseconds
#define ROUNDS            10      //rounds played
#define MIN_SLEEP         1000    //minimum sleep time in milliseconds
#define MAX_INPUT_LEN     32      //max input length on guess

//necessary libs
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  //declare necessary variables
  unsigned long sleep_time, guess_time;
  char read_buf[MAX_INPUT_LEN];
  float time_dif = 0.0;
  int err_check = 0;
  int dif_ints, dif_dec;

  //print out instructions

  //loop the game
  int i;
  for(i = 0; i < 10; i++) {
    //get a psuedo random time to sleep

    //sleep for that time


	  //print out the user prompt
	  write(STDOUT_FILENO, "~ ", 2);

	  //call read to let the user type in the prompt
	  //make sure to provide a sensical max length into read
	  err_check = read(STDIN_FILENO, read_buf, (size_t) MAX_INPUT_LEN);

	  //check that read didn't crash
	  if(err_check < 1) {
		  return -1;
	  }

	  //get the difference between sleep time and guess time
	  diff = sleep_time - guess_time;

	  //if negative make positive for scoring


	  //print how long it took them to type to tenth of second
	  dif_ints = time_dif/1000;
	  dif_dec = (time_dif/100) - (dif_ints*10);
	  printf("You took %d.%d\n", dif_ints, dif_dec);
  }

  return 0;
}
