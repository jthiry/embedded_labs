#include <stdio.h>
#include "math.h"

int main(){

    char op;
    int first = 0;
    int second = 0;

    while(1){

        printf("\nCalculator running... Enter input: number command number\n");
        
        if( scanf( "%d %c %d", &first, &op, &second ) < 0 ) return 0;
        

        printf( "%c", op ) ;

        if( op == '+' )
        {
            printf( "%d", add(first,second));
            continue;
        }
        else if( op == '-' )
        {
            printf("%d", sub(first,second));
            continue;
        }
        
        else if( op == '*' )
        {
            printf("%d", mul(first,second));
            continue;
        }
        
        else if( op == '/' )
        {
            printf( "%d", div(first,second));
            continue;
        }
        
        else if( op == '%' )
        {
            printf("%d", mod(first,second));
            continue;
        }
        else
        {
            return 1;
        }
          
    } 

}
