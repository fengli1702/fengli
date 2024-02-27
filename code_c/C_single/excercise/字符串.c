#include <stdio.h>
#include <stdlib.h>

void invert(char a[]){
    char ch;
    scanf("%c",&ch);
    static int i=0;
    if(ch!='.'){
        invert(a);
        a[i++]=ch;
    }
    a[i]='\0';
}
int main(){
    char a[100]={};
    invert(a);
    for(int m=0;m<=5;m++)
        printf("%c ",a[m]);
}