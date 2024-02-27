#include <stdio.h>
int main(){
    unsigned int  x;
    x=4294967295;
    x=-x;
    printf("%u,%d",x,x);
}