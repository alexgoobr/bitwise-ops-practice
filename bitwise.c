/*
  RULES:

  Your code must conform to the following style:

  int Funct(arg1, arg2, ...) {
    int var1 = Expr1;
    ...
    int varM = ExprM;

    varJ = ExprJ;
    ...
    varN = ExprN;
    return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.
 */

 /*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /*0 is the only value of x where both x and -x (~x + 1) will have the leftmost bit as 0.*/
  return (((~x + 1) | x)>>31) + 1;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /* Divide and Conquer strategy
   * Creating 2-bit fields with the sum of the bits in corresponding spots
   * Then doing the same with 4-bit fields (using the 2-bit fields) and so on.*/
  int by2 = (0x55<<24) + (0x55<<16) + (0x55<<8) + 0x55; //0x55555555
  int by4 = (0x33<<24) + (0x33<<16) + (0x33<<8) + 0x33; //0x33333333
  int by8 = (0x0F<<24) + (0x0F<<16) + (0x0F<<8) + 0x0F; //0x0F0F0F0F
  x += ~((x >> 1) & by2) + 1;
  x = (x & by4) + ((x >> 2) & by4);
  x = (x + (x >> 4)) & by8;
  x += (x >> 8);
  x += (x >> 16);
  return x & 0x3F;
}
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  /*DeMorgan's Law*/
  return ~((~x)&(~y));
}
/*
 * bitRepeat - repeat x's low-order n bits until word is full.
 *   Can assume that 1 <= n <= 32.
 *   Examples: bitRepeat(1, 1) = -1
 *             bitRepeat(7, 4) = 0x77777777
 *             bitRepeat(0x13f, 8) = 0x3f3f3f3f
 *             bitRepeat(0xfffe02, 9) = 0x10080402
 *             bitRepeat(-559038737, 31) = -559038737
 *             bitRepeat(-559038737, 32) = -559038737
 *   Legal ops: int and unsigned ! ~ & ^ | + - * / % << >>
 *             (This is more general than the usual integer coding rules.)
 *   Max ops: 40
 *   Rating: 4
 */
int bitRepeat(int x, int n) {
  /*Creates an integer with only the lower n bits set to the corresponding values of x, rest are 0.
   *Copies the n bits, located at [0, n-1] to [n, 2n-1]
   *Then copies the bits at [0, 2n-1] to [2n, 4n-1] and so on until we hit 32*/
  int n32_check = n/32;
  int rep = x - (!(n32_check))*((x>>n)<<n);
  int w = n;
  rep += !(w/32)*(rep << (w%32));
  w = w<<1;
  rep += !(w/32)*(rep << (w%32));
  w = w<<1;
  rep += !(w/32)*(rep << (w%32));
  w = w<<1;
  rep += !(w/32)*(rep << (w%32));
  w = w<<1;
  rep += !(w/32)*(rep << (w%32));
  return rep;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  /*We make sure the last bit we need is a 0, and then right shift by n-1. If there are any bits left that we missed the result will be != 0*/
  int sign = x>>31;
  return !(((~x & sign)|(x & ~sign))>>(n + ~0));
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /*Multiply n by 8 (bytes -> bits), then right shift x so the n-th byte becomes the 0th byte. Then only return the 0th byte.*/
  return 0xFF&(x>>(n<<3));
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /* Checks if signs are same or opposite
   * If signs are opposite we simply check if x is negative (which means it
   * has to be less, or positive (which means it has to be greater).
   * If signs are same we check if the difference y-x is positive (y is bigger) or negative (x is bigger).
   **/
  int opposite_sign = ((x^y)>>31);
  int x_sign = x >> 31;
  int diff = ~((y+(~x+1))>>31);
  return !!(x_sign & opposite_sign) | !!(diff & ~opposite_sign);
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  /*First reduces x by 1 and then just handles it as if the question was about x >= 0. Also handles case for x = INT_MIN*/
  int minus_one = x + ~0;
  int sign = (minus_one>>31) | (x>>31); // | (x>>31) for INT_MIN case
  return !(sign&1);
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  /*Creates a mask of the the bits we want to set to 0, and also one that covers the case where n is zero.
   *Then does an arithmetic right shift and applies the masks to set the n bits to 0.*/
  int mask = (~0)<<(32+(~n+1));
  int n0_mask = (1 + ~(!!n)); //0 if n is 0, 1 if n is not 0
  return (x>>n)&~((mask)&(n0_mask));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /* Tmin is 100..000 so we just left shift a 1 to the MSB-position*/
  return 1<<31;
}