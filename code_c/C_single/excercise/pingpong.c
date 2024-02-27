#include <stdio.h>
#include <stdlib.h>
#define N 20
int direction=1;
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
void judge_8(int number){
    //传入number即f(N),判断性质来决定下一个,direction=-1是向下
    int num=number;
    int check1=0,check2=0;
    while(num>0)           //and7检测是否为能被8整除的数
        num-=8;
    if(num==0)
        check1=1;
    while(number){
        int check=subsubsub(number,10);
        if(check==8)
            check2=1;
        number=subsub(number,10);
    }
    if(check1==1||check2==1){
        direction=-direction;
        return;
        }
    return;
}
int next_f(int f){
    int f_next;
 //   judge_8(f,&direction);
    judge_8(f);
    if(direction==1)
        f_next=multi(2,f)+2;
    else
        f_next=multi(2,f)-2;
    return f_next;
}
void  get_fn(int choice){
    int i=2;
    int f[N+1]={0};
    f[1]=3;
    f[2]=8;
    while(i<N){
        f[i+1]=next_f(f[i]);
        if(f[i+1]>=4096)
            f[i+1]-=4096;
//        printf("%d ",f[i]);
        i++;
    }
    printf("%d",f[choice]);
}

int main(){
//    printf("%d",multi(10,0));
    get_fn(7);
}