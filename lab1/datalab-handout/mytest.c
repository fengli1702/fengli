#include <stdio.h>
int isAsciiDigit(int x) {
  int a=0x30;
  int b=0x3a;
  a=~a+1;
  b=~b+1;
  return !((x+a)>>31)&((b+x)>>31);  //x>=a && x<=b  -> !(x-a)>>31 && !(b-x)>>31
}
int isLessOrEqual(int x, int y) {
  int a=~x+1;
  int b=y+a;   //if x<=y  :b>=0 b[31]==0
         //b>>31=0
  return !(b>>31);
}
int logicalNeg(int x) {

  int a=~x+1;
  int b=a|x;  //if x==0 :b==0 else b!=0
  b=~(b>>31);
  return b&1;
}
int bitXor(int x, int y) {
  return  ~((~(x&~y))&(~(~x&y)));
}
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
int main(){
    int oddbits=0x3a;

//    printf("%x \n",howManyBits(12));
//    printf("%x \n",howManyBits(298));
//    printf("%x \n",howManyBits(-5));
//    printf("%x \n",howManyBits(0));
    printf("%x \n",logicalNeg(-2147483648));
}