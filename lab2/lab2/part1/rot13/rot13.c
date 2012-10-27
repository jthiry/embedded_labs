/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Joe Battaglia <jabat295@gmail.com>
 *          Hans Reichenback <HansReich25@gmail.com>
 *	    Josh Thiry <josh.thiry@gmail.com>
 * Date:    10/14/2012
 */
#include <stdlib.h>
#include <errno.h>

//function to do the rot13 rotation
char rotate(char);
int main(int argc, char* argv[]) {

	char my_buff[128];		//buffer to hold the input string
	int num_chars, i, chk;
	int len;
	int found;
	char* tmp;
	for(i=0; i < argc; i++)
	{
		len = 0;
		tmp = argv[i];
		found = 0;

		while (!found)
		{
			if(tmp[len] == '\0') found = 1;
			else len++;
		}
		if( write(1, argv[i], len) < 1) exit(1);
	}
/*
	for(i=0; i < argc; i++)
	{
		printf("\targv[%d]=%s\n\thex=%x\n\t*argv[]=%x\n",i,argv[i], argv[i], *argv[i]);
	}
*/
  //indefinite loop
  while( 1 )
  {

	  num_chars = read(0, my_buff, 128);	//calling the read SWI to read in input
	  //printf("nchar=%d\n",num_chars);
	  if(num_chars == 0) exit(0);		//if return length is 0, nothing was entered
	  if(num_chars < 0) exit(1);		//if return length < 0, error

	  //loop through the whole input str (minus the last char)
	  for(i = 0; i < num_chars - 1; i++)
	  {
		  //printf("%d\n", i ) ;
		  my_buff[i] = rotate(my_buff[i]);	//call the rot14 function for current char
	  }

		chk = write(1, my_buff, num_chars); 	//calling write SWI to output newly rot13 str

		//if return value < 0, error
		if (chk < 0 )
		{
			//printf("%d\n", errno);
			exit(1);
		}

	}
	return 0;	//program will never get to here
}
char rotate(char rot_me)
{

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
