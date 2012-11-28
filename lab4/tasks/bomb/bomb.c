/* @file bomb.c: Prompts the user with a scenario for defusing a bomb. Must guess when the timer is 
 *		betweetn 25-30 seconds. This will test sleep, timer, and read.
 *
 *      Authors:
 *              Joe Battaglia <JABAT295@gmail.com>
 *              Hans Reichenbach <HansReich25@gmail.com>
 *              Josh Thiry <josh.thiry@gmail.com>
 *
 * Links to libc.
 */



#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{	
	char read_buf[1];
	int err_check;
	unsigned long start_time, end_time;
	unsigned long allow_read_prompt = 10000;
	
	//Print on the scenario to the user
	puts("\n\n");
	puts("UH OH! You have triggered a bomb!\n");
	puts("The bomb can only be disabled between 15-20 seconds.\n");
	puts("The bomb will alert you when the time has started.\n");
 	puts("Press any key when you want to disable the bomb.\n");
	puts("Be careful! Press to early or wait to long, you go BOOM!\n");

	//give the user time t read scenario before the timer starts
	sleep(allow_read_prompt);
	
	//Inform user that the timer is starting
	puts("Timer starting now!!\n");

	//Start the timer
	start_time = time();

	//wait for input from the user
	err_check = read(STDIN_FILENO, read_buf, 1);

	//Make sure read functioned properly
	if(err_check < 1) {
                  return -1;
          }

	//end the timer
	end_time = time();

	//check to see if user pressed the timer too early
	if((end_time - start_time) < 15000){
		printf("You pressed to early! The time had only been, %lu seconds! 3,2,1....BOOM!\n", ((end_time - start_time)/1000));
	}

	//check and see if user was correct
	else if((end_time - start_time) > 15000 && ((end_time - start_time) < 20000)){
		printf("Congratualtions! You defused the bomb! The time was, %lu seconds.\n", ((end_time - start_time)/1000));
	}

	//check and see if user waited too long
	else{
                printf("You pressed to late! The time had been, %lu seconds! 3,2,1....BOOM!\n", ((end_time - start_time)/1000));
        }

	return 0;
}

