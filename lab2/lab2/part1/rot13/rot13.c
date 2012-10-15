/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Group Member 1 <email address>
 *          Group Member 2 <email address>
 * Date:    The current time & date
 */
#include <stdlib.h>
#include <errno.h>

char rotate(char);
int read( int d, void *buff, size_t s);
int write( int d, const void *buff, size_t s);

int main(void) {

	char my_buff[128];
	int num_chars, i, chk;
	
	while( 1 ) 
	{
		num_chars = read(0, my_buff, 128);
		//printf("nchar=%d\n",num_chars); 
		if(num_chars == 0) exit(0);
		if(num_chars < 0) exit(1);

		for(i = 0; i < num_chars - 1; i++)
		{
			//printf("%d\n", i ) ;
			my_buff[i] = rotate(my_buff[i]);
		}

		chk = write(1, my_buff, num_chars); 
		if (chk < 0 )
		{
			//printf("%d\n", errno);			
			exit(1);
		}
		//done?
	}	
	return -255;
}
char rotate(char rot_me)
{
	int x = (int) rot_me;
	if( (x < 65) || (x > 122) ) exit(1);
	if( (x < 97) && (x > 90) ) exit(1);
	//in bounds char

	if( x < 78 )
		rot_me += 13;
	else
	{
		if( x < 97 || x > 109)
			rot_me -= 13;
		else
			rot_me += 13;
	}

	return rot_me;
}
 
