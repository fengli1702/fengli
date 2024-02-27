#include <stdio.h>
#include <stdlib.h>
#define max 10
//循环队列
typedef struct {
    int *base;//储存空间基地址
    int front ;//头指针
    int rear ;//尾指针
}SqQueue;
void initqueue(SqQueue *q){
    q->base=(int *)malloc(sizeof(int)*max);
    if(!q->base)  return;
    else{
        q->front=q->rear=0;
        return ;
    }
}
int qlength(SqQueue *q){
    return (q->front-q->rear+max)%max;
}
//入队
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
//出队
int  outQueue(SqQueue *q){
    if(qlength(q)==0){
        printf("NULL");
        return -1;
    }
    else{
        int data;
        data=q->base[q->front];
        q->front=(q->front+1)%max;
        printf("\nout:%d",data);
        return data;
    }
}
//取头元素
void gethead(SqQueue *q){
    if(q->front!=q->rear)
        printf("\ngethead:%d", q->base[q->front]);
    return;
}
int main(){
    SqQueue p,*q=&p;
    initqueue(q);
    for(int i=0;i<5;i++)
        enQueue(q,i);
    printf("\nlength:%d\n",qlength(q));
    int data;
    data=outQueue
    (q);
    gethead(q);
}