#include<stdio.h>
#include<stdlib.h>
#define M  10
#define N 10
static int i=0;
int ack(int m,int n){
    if(m==0){
        i++;
        printf("%d m==0计算n+1,m==%d,n==%d\n",i,m,n);
        return n+1;
    }
    if(n==0&&m!=0){
        i++;
        printf("%d n==0&&m!=0计算ack(m-1,1),m==%d,n==%d\n",i,m,n);
        return ack(m-1,1);}
    else{
        i++;
        printf("%d :else计算ack(m-1,ack(m,n-1)):m==%d,n==%d\n",i,m,n);
        return ack(m-1,ack(m,n-1));}
}
int Ack(int m,int n){
    int ackm[M][N]={};
    int i,j;
    for(j=0;j<N;j++)
        ackm[0][j]=j+1;//给(0,j)ackmann数赋初值
    for(i=1;i<=m;i++){
        ackm[i][0]=ackm[i-1][1];//条件二
        for(j=1;j<N;j++)
            ackm[i][j]=ackm[i-1][ackm[i][j-1]];//条件三
    }
    return ackm[m][n];
}
int main(){
    printf("%d",ack(2,1));
    printf("  %d",Ack(2,1));
}