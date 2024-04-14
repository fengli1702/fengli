/* 
 * CS:APP Data Lab 
 * 
 * <PB22111702 李岱峰>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1   0100 0101   
 *   Legal ops: ~ &    a^b=(a&~b)|(~a&b)
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return  ~((~(x&~y))&(~(~x&y)));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1<<31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise               0111 1111 1111 1111 1111 1111 1111 1111
 *   Legal ops: ! ~ & ^ | +     
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {

  int Tmax=0;
  Tmax=x+1;         //x=-1
  Tmax=~Tmax;       //如果Tmax==x，则返回1  Tmax^x=0
  return (!(x^Tmax))&!!(x+1);   //排除-1

}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1  D=1101 A=1010
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int oddbits=0xAA;
  oddbits=(oddbits<<8)+0xAA; //1010 1010 1010 1010 1010 1010 1010 1010
  oddbits=(oddbits<<8)+0xAA; //0xAAAAAA
  oddbits=(oddbits<<8)+0xAA;
  return !((oddbits&x)^oddbits);  //a==b  -> !a^b
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.     0001  -> ffff  0010 -> fffe
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return  ~x+1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int a=0x30;
  int b=0x3a;
  a=~a+1;
  b=~b+1;
  return (!((x+a)>>31))&((b+x)>>31);  //x>=a && x<=b  -> !(x-a)>>31 && !(b-x)>>31
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {  //x==0?z:y   x!=0  -> 0xffffffff|y  x==0 -> 0x00000000 ->~x|z
  int a=!!(x^0); //a=0 if x=0 else a =1
  int b=~a+1;    //b=0xffffffff if x=0 else b=0x00000000
  int c=~(y&~b)+1;
  int d=~(z&b)+1;
  return y+z+c+d;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int a=~x+1;
  int b=y+a;   //if x<=y  :b>=0 b[31]==0
         //b>>31=0
  return !(b>>31);
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1   0001->1110+1=1111  0000->1111+1=0000
 *   Legal ops: ~ & ^ | + << >>                        ffff->0000+1=0001 ->fffe
 *   Max ops: 12                                         000100->1111011+1=11111100 ->11111000
 *   Rating: 4 
 */
int logicalNeg(int x) {

  int a=~x+1;
  int b=a|x;  //if x==0 :b==0 else b!=0
  b=~(b>>31);
  return b&1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5    12=  0000000...0   01100
 *            howManyBits(298) = 10  298= 000.......0 0100101010
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    int b16,b8,b4,b2,b1,b0;
    int flag=x>>31;
    x=(flag&~x)|(~flag&x);   //x>=0不变 ,x<0按位取反
    b16=(!!(x>>16)) <<4;       //如果高16位不为0,则我们让b16=16
    x>>=b16;                   //如果高16位不为0 则右移动16位 来看高16位的情况
    b8=(!!(x>>8))<<3;
    x >>= b8;
    b4 =(!!(x>>4))<<2;
    x >>= b4;
    b2=(!!(x>>2))<<1;
    x >>= b2;
    b1=!!(x>>1);
    x>>= b1;
    b0=x;
  return b0+b1+b2+b4+b8+b16+1;//加1是因为符号位,其他的是从低开始1的位数，从后向前找，不重不漏
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
    unsigned exp=(uf&0x7f800000)>>23;
    unsigned sign=uf>>31&0x1;
    unsigned frac=uf&0x7FFFFF;
    unsigned res;
    if(exp==0xFF)return uf;
    else if(exp==0){
        frac=frac<<1;
        res=(sign<<31)|frac|(exp<<23);
    }
    else{
     exp++;
     res=(sign<<31)|frac|(exp<<23);
   }
   return res;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  //？？？
  //float->int
  unsigned exp=(uf&0x7f800000)>>23;
  unsigned frac=uf&0x007fffff;
  int  sign=!!(uf&0x80000000); //+-1
  int E=exp-127;   //获得指数，放大倍数
  if(E<0){
    //这是一个小数，返回(int ) 0
    return 0;
  }
  else{
    //这是一个整数
    if(E>=31){
      //超出范围（int32）
      return 0x80000000u;
    }
    else{
      //在范围内
      frac=frac|0x00800000;  //1.0+frac  得到1.111111...
      if(E>23){
        frac=frac<<(E-23);   //已经占有23位，还需要E-23位
      }
      else{
        frac=frac>>(23-E);    //需要右移23-E位
      }
      if(sign==1){
        return ~frac+1;
      }
      else{
        return frac;
      }
    }
  }
  return 0x80000000u;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    if(x>127){
        return 0xFF<<23;
    }
    else if(x<-148) return 0;
    else if(x>=-126){
        return ((x+127)<<23);
    } 
    else{
        return (1<<(148+x));
    }
}
