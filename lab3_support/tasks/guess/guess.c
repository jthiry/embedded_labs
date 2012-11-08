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
#define SEED              12345   //seed for random number

//necessary libs
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  //declare necessary variables
  unsigned long sleep_time, guess_time, round_score;
  unsigned long total_score = 9000;
  char read_buf[MAX_INPUT_LEN];
  int dif_ints, dif_dec, err_check;

  //print out instructions
  printf("This game is scored like golf. I'm going to sleep for a random\n");
  printf("period of time and your goal is to guess how long it was! The\n");
  printf("lower your score after 10 rounds the better! Ready?\n\n\n");

  //loop the game
  int i;
  for(i = 0; i < 10; i++) {
    //get a psuedo random time to sleep
    long rando = time();
    do {
      rando = (SEED*rando) % MAX_SLEEP;
    } while(rando < MIN_SLEEP);

    //User instructions
    printf("Press any key when you are ready for this round...\n\n");
    read(STDIN_FILENO, read_buf, 1);

    //sleep for the random time
    printf("Sleeping.....\n");
    sleep(rando);

	  //print out the user prompt
	  printf("Guess (in tenths of a second, no decimal): ");

	  //call read to let the user type in their answer
	  //make sure to provide a sensical max length into read
	  err_check = read(STDIN_FILENO, read_buf, (size_t) MAX_INPUT_LEN);

	  //check that read didn't crash
	  if(err_check < 1) {
		  return -1;
	  }

	  //convert input to number
	  guess_time = strtoul(read_buf, (char **)(read_buf+err_check), 10);

	  //shift rando to tenths of a second
	  sleep_time = rando/100;

	  //get the difference between sleep time and guess time
	  if(sleep_time > guess_time) {
  	  round_score = sleep_time - guess_time;
  	} else {
  	  round_score = guess_time - sleep_time;
  	}

	  //print their guess
	  dif_ints = guess_time/1000;
	  dif_dec = (guess_time/100) - (dif_ints*10);
	  printf("You guessed %d.%ds\n", dif_ints, dif_dec);

	  //print the actual time
	  dif_ints = sleep_time/1000;
	  dif_dec = (sleep_time/100) - (dif_ints*10);
	  printf("It actually was %d.%ds\n", dif_ints, dif_dec);

	  //add to their total score and print current score
	  total_score += round_score;
	  printf("Your round score was %lu\n\n", round_score);
  }

  printf("GAME OVER\n\nYour final score was....\n%lu!!!", total_score);

  return 0;
}
