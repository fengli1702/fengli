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

int  cal_if(int num){  //如果返回1则为奇数，返回0则为偶数
    if(subsubsub(num,2))
        return 1;
    return 0;
}
int odd(int num){
    //奇数处理
    int ans=0;
    int oldnum=num;
    int caltime=16;
    while(caltime>0){//printf("%d\n",subsubsub(oldnum,2));
        if(subsubsub(oldnum,2)==0)
            ans+=1;
        oldnum=subsub(oldnum,2);
        caltime--;
    }
    return ans;
}
int even(int num){
    num=65536-num;
    //printf("%d\n",num);
    int ans=0;
    int oldnum=num;
    int caltime=16;
    while(caltime>0){
        if(subsubsub(oldnum,2)==0)
            ans+=1;
        oldnum=subsub(oldnum,2);
        caltime--;
        
    }
    return ans;
}
int finailcount(int num){
    int ans=0;
    if(cal_if(num)==1)
        ans=odd(num);
    else
        ans=even(num);
    return ans;
}
int main(){
    
    printf("%d",finailcount(6280));
}
