#include <stdio.h>

int multi(int a,int b){
    //a*b
    if(a==0||b==0)
        return 0;
    int num=b-1;
    int olda=a;
    while(num>0){
        a+=olda;
        num--;
    }
    return a;
}
int subsub(int a,int b){
    //  a/b
    //b+b+b....>=a时返回mulb-b
    int mulb=0;
    for(int i=1;i<=a;i++){
        mulb+=b;
        if(mulb==a)
            return i;
        else if(mulb>a)
            return i-1;
    }
    return 0;
}
int subsubsub(int a,int b){
    //  a%b
    int num;
    num=subsub(a,b);
    int subsub=a-multi(num,b);
    return subsub;
}

int strcmp_my(char * str1,char *str2,int length){
    int i=0;
    for(i;i<length;i++){
        if(str1[i]!=str2[i])
            return 0;
    }
    return 1;
}
