#include <stdio.h>
#include <stdlib.h>
#define max 10
//循环队列
typedef struct {
    int *base;//储存空间基地址
    int front ;//头指针
    int rear ;//尾指针
}SqQueue;
SqQueue* initqueue(){
    SqQueue* q=(SqQueue *)malloc(sizeof(SqQueue));
    q->base=(int *)malloc(sizeof(int)*max);
    if(!q->base)  return q;
    else{
        q->front=q->rear=0;
        return q;
    }
}
int qlength(SqQueue *q){
    return (-q->front+q->rear+max)%max;
}
//从队尾入队
void enQueue(SqQueue *q,int data){
    if((q->rear+1)%max==q->front){//队满
        printf("error!");
        return ;
    }
    else{
        q->base[q->rear]=data;
        q->rear=(q->rear+1)%max;
        return;
    }
}
//从队尾删除
void out_from_the_tail(SqQueue *q){
    if(qlength(q)==0){//合法检验
        printf("NULL");
        return ;
    }
    else{
        int data;
        data=q->base[(q->rear-1+max)%max];
        q->rear=(q->rear-1+max)%max;
        printf("\n从队尾删除:%d",data);
    }
}
//从队头出队
int  outQueue(SqQueue *q){
    if(qlength(q)==0){
        printf("NULL");
        return -1;
    }
    else{
        int data;
        data=q->base[(q->front+max)%max];
        q->front=(q->front+1)%max;
        printf("\n从队头出队:%d",data);
        return data;
    }
}
//从队头入队
void in_from_the_front(SqQueue *q,int data){
    if((q->rear+1)%max==q->front){//队满
        printf("error!");
        return ;
    }
    else{
        q->base[(q->front-1+max)%max]=data;
        q->front=(q->front-1+max)%max;
        return;
    }
}
//打印队列
void traversal(SqQueue *q){
    int pointer;
    pointer=(q->front+max)%max;
    for(;(q->rear)%max!=pointer;){
        printf("%d ",q->base[pointer]);
        pointer=pointer+1;
        pointer=(pointer)%max;
    }
}
int main(){
    SqQueue*p= initqueue();
    printf("length1:%d",qlength(p));
    for(int i=0;i<5;i++)
        enQueue(p,i);
    printf("\n");
    traversal(p);
    for(int i=5;i<9;i++)//队头入队
        in_from_the_front(p,i);
    printf("\n::tra");
    traversal(p);
    printf("\nlength1:%d",qlength(p));
    out_from_the_tail(p);//从队尾删除
    printf("\n::tra");
    traversal(p);
    printf("\nlength1:%d",qlength(p));
    outQueue(p);
    printf("\n::tra");
    traversal(p);
    printf("\nlength1:%d",qlength(p));
}