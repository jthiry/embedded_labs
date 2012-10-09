#include <stdio.h>
#include <math.h>

int main(){

    char op;
    int first = 0;
    int second = 0;
    int keep_going = 1;

    int add(int x, int y);
    int sub(int x, int y);
    int mul(int x, int y);
    int div(int x, int y);
    int mod(int x, int y);

    while( keep_going == 1){

        printf("\nCalculator running... Enter input: number command number\n");
        
        if( scanf( "%d, %c, %d", &first, &op, &second ) < 0 ) return 0;
        

        printf( "%c", op ) ;

        if( op == '+' )
        {
            printf( "%d", add(first,second));
             continue;
        }
        
        if( op == '-' )
        {
            printf("%d", sub(first,second));
             continue;
        }
        
        if( op == '*' )
        {
            printf("%d", mul(first,second));
             continue;
        }
        
        if( op == '/' )
        {
            printf( "%d", div(first,second));
             continue;
        }
        
        if( op == '%' )
        {
            printf("%d", mod(first,second));
        }
        else
        {
            return 1;
        }
        
        
    } 

    return 0;

}
