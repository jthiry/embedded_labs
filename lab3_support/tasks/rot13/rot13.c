/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Joe Battaglia <jabat295@gmail.com>
 *          Hans Reichenback <HansReich25@gmail.com>
 *	    Josh Thiry <josh.thiry@gmail.com>
 * Date:    10/14/2012
 */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//function to do the rot13 rotation
char rotate(char);
int main(int argc, char* argv[]) {

	char my_buff[128];		//buffer to hold the input string
	int num_chars, i,chk;

	//DEBUG
	//puts("DEBUG--about to check the args in rot\n");
	puts("rot13.c::main++\n");

	//print out the arguments from argv
	/*
	int i; 
	puts("DEBUG--writing the args in rot\n");
	for(i=0; i < argc; i++)
	{
		printf("argv[%d]=%s\n",i , argv[i]);
	}
	*/


	//loop until user exits with CR or ctrl-D
	while( 1 )
	{
		//DEBUG
		puts("rot13.c::main::loop++\n");

		num_chars = read(STDIN_FILENO, my_buff, 128);	//calling the read SWI to read in input
		puts("rot13.c::main::loop::read syscall complete\n");

		//DEBUG
		if(num_chars == 0) exit(0);		//if return length is 0, nothing was entered
		if(num_chars < 0) exit(1);		//if return length < 0, error

		//DEBUG
		puts("DEBUG::rot13--after exit check in loop\n");

		//loop through the whole input str (minus the last char)
		for(i = 0; i < num_chars - 1; i++)
		{
			my_buff[i] = rotate(my_buff[i]);	//call the rot14 function for current char
		}

		chk = write(1, my_buff, num_chars); 	//calling write SWI to output newly rot13 str

		//if return value < 0, error
		if (chk < 0 )
		{
			exit(1);
		}

	}
	return 0;	//program will never get to here
}
char rotate(char rot_me)
{
  //DEBUG
  puts("DEBUG::rot13--rotating shitake mushrooms\n");

	int x = (int) rot_me;			//getting ascii value of char
	if( (x < 65) || (x > 122) ) exit(1);	//value out of range, error
	if( (x < 97) && (x > 90) ) exit(1);	//value out of range, error
	//in bounds char

	//char is A-M
	if( x < 78 )
		rot_me += 13;	//add 13 to it
	else
	{
		//char is either N-Z or n-z
		if( x < 97 || x > 109)
			rot_me -= 13;	//subtract 13
		//else char is a-m
		else
			rot_me += 13;	//add 13
	}

	return rot_me;	//return the newly rotated char
}
