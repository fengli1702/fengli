#include <stdio.h>
#include <stdlib.h>
//汉诺塔问题
int m=0;
void move(char a,int n, char c){
    printf("\nstep:%d,%d,%c->%c",++m,n,a,c);
}
void hanoi(int n,char a,char b,char c){
    if(n==1)
        move(a,1,c);
    else{
        hanoi(n-1,a,c,b);
        move(a,n,c);
        hanoi(n-1,b,a,c);
    }
}
int main(){
    char a='A',b='B',c='C';
    hanoi(4,a,b,c);
}