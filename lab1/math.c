/* 
 * Simple calculator functions for use on a Gumstix board.
 *
 * @authors Hans Reichenbach, Joe Battaglia, Josh Thiry
 */

#include <math.h>

// adds x and y together and returns the result
int add(int x, int y) {
  return x + y;
}

// subtracts y from x and returns the result
int sub(int x, int y) {
  return y - x;
}

// multiplies x and y and returns the result
int mul(int x, int y) {
  return x*y;
}

// divides x by y and returns the integer result, ignoring the remainder
int div(int x, int y) {
  return x/y;
}

// returns the remainder of x divided by y
int mod(int x, int y) {
  return x%y;
}
