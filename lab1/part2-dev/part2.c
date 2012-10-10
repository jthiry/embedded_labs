#include <stdlib.h>
#include <stdio.h>

/**
 * Authors: Joe Battaglia, Hans Reichenbach, Josh Thiry
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
	int i, j;
	int foundInner;
	int result = 0;

	for (i = 0; i < len; i++) {
		foundInner = 0;
		for (j = 0; j < len; j++) {
			if (i == j) {
				continue;
			}
			if (arr[i] == arr[j]) {
				foundInner = 1;
			}
		}
		if (foundInner != 1) {
			result = arr[i];
		}
	}

	return result;
}
#endif


#ifdef OPTIMIZE1
/* This function takes the time of oddball from O(n^2) to O(n).
   It takes advantage of XOR to determine the oddball.
   Since the duplicates will be XOR'ed twice, they will give a 
   0 output. We will then be left with the number that wasn't
   duplicated (since it was XOR'ed with 0).
*/
int oddball(int *arr, int len) {
	/* Put your code here */
    int  i, result = 0;
    //loop to go through the 2n - 1 array
    for( i = len; i >= 0; i-- )
    {
        result = result ^ arr[i]; //XORing reult with current value of arr
    }
	return result; //returning the oddball
}
#endif


#ifdef OPTIMIZE2
/*This function takes the time of oddbal from O(n^2) to O(2n).
  It uses an interger array that will determine which numbers have duplicates
  insteading of using the nested for loop. 
*/
int oddball(int *arr, int len) {
	/* Put your code here */
    int  i;
    int tmp[(len/2)+1];	//array to determine nonduplicate int
    //loop to go through entire 2n - 1 array
    for( i = len; i >= 0; i--)
    {
	tmp[arr[i]-1]++;	//incrementing place holder by 1 (duplicacy check)
    }
    for(i = len; i >= 0; i--)	//loop to determine which number isn't dupicated
    {
	//if a placeholder is only 1, then it doesn't have a duplicate
	if(tmp[i] == 1)
		return i+1;	//reuturn the oddball
    }
    return i + 1; //return to make compiler happy
}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */
#ifndef RNG
int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand()%(len/2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while(rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}

#endif


