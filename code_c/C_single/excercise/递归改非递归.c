#include <stdio.h>
#include <stdlib.h>
int gcd(int a,int b){
    if(!b) return a;
    else{
        int nxt=a%b;
        int x=gcd(b,nxt);
        return x;
    }
}

struct gcd_{
    int a;
    int b;
    int x;
    int nxt;
}gcd_[20];
int gcd2(int a,int b){
    int top=0;//初始化栈
    int x=0,nxt=0;
    while(b!=0){//递归终止条件
        gcd_[top].a=b;
        gcd_[top].b=a%b;
        gcd_[top].x=x;//栈内保存元素
        top++;
        nxt=a%b;//更新形参
        a=b;
        b=nxt;
    }
    gcd_[--top].x=a;//压入终止条件
    while(top!=0){
        gcd_[top-1].x=gcd_[top].x;
        top--;
    }
    return gcd_[0].x;
}

struct ack_{
    int m;
    int n;
    int res;
}ack_[50];
int ack2(int m,int n){
    int top=0;//初始化栈
    int res=0;
    do{//递归终止条件
        if(m==0){
        ack_[top-1].res=ack_[top-1].n+1;//压入终止条件
        res=ack_[top-1].res;printf("ok3");
        top--;
        while(top!=0){
        ack_[top-1].res=ack_[top].res;
        top--;printf("ok4");
        } }
        if(m!=0&&n==0){
        printf("ok1");
        m=m-1;//更新形参
        n=1;
        res=res;}
        if(m!=0&&n!=0){
        ack_[top].m=m-1;
        ack_[top].n=ack_[top].res;
        ack_[top].res=res;//栈内保存元素
        top++;printf("ok2");
        m=m-1;//更新形参
        n=1;
        res=res;

        ack_[top].m=m;
        ack_[top].n=n-1;
        ack_[top].res=res;//栈内保存元素
        top++;
        m=m;//更新形参
        n=n-1;
        res=res;}
    printf("m:%d,n:%d\n",m,n);
    }while(top!=0);
    return ack_[0].res;
}
int main(){
 //   int n=15;
    printf("%d  ", ack2(2,1));
//    printf("%d  ",gcd2(5,n));
}