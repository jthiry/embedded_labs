#include <stdio.h>
#include "math.h"

int main(){

  //necessary variable creation
  char op;
  int first = 0;
  int second = 0;
  int scanRet = 0;

  //infinite loop to get commands
  while(1){

    printf("\nCalculator running... Enter input: number command number\n");

    //get the numbers and operand from the user input
    scanRet = scanf( "%d %c %d", &first, &op, &second );

    /* DEBUG*/
    printf("scanRet value is %d\n", scanRet);
    printf("first: %d\n", first);
    printf("op: %c\n", op);
    printf("second: %d\n\n", second);

    if( scanRet < 1 ) return 0;

    //figure out which operand was used and run the appropriate command
    if( op == '+' )
    {
      printf( "%d", add(first,second));
      continue;
    } else if( op == '-' )
    {
      printf("%d", sub(first,second));
      continue;
    } else if( op == '*' )
    {
      printf("%d", mul(first,second));
      continue;
    } else if( op == '/' )
    {
      printf( "%d", div(first,second));
      continue;
    } else if( op == '%' )
    {
      printf("%d", mod(first,second));
      continue;
    } else
    {
      return 1;
    }
  }

  return 0;

}
