//单链表，整形，递归的表示 1.求最大整数  2.求链表结点个数  3.求所有整数平均值
#include <stdio.h>
#include <stdlib.h>
typedef int elemtype;
//ADT数据类型
typedef struct node{
    elemtype data;
    struct node *next;
}Linknode,*Linklist;
Linklist initlist(){//初始化
    Linknode* p=(Linknode *)malloc(sizeof(Linknode));
    if(!p){//分配失败
        printf("error!");
        exit(0);
    }
    p->data=0;
    p->next=NULL;
    return p;      //返回初始化的指针
}
void listadd(Linklist p){//尾插法建立链表
    elemtype insert;
    printf("一个一个输入要插入的元素,元素间空格或每输入一个后换行,以-1结尾");
    scanf("%d",&insert);
    Linklist q=p;
    while(q->next)
        q=q->next;
    while(insert!=-1){
        Linklist new_point=(Linknode *)malloc(sizeof(Linknode));
        new_point->data=insert;
        new_point->next=NULL;
        if(q==NULL){
            p=new_point;
            q=p;}
        else {
            q->next=new_point;
            q=new_point;
        }
        scanf("%d",&insert);
    }
}
//求最大整数
void find_max(Linklist head,elemtype *max){
    if(head==NULL){
        return;
    }
    if(head->data>(*max))
        *max=head->data;
    find_max(head->next,max);
    return;
}
//求节点个数
void find_num(Linklist head,int *num){
    if(head==NULL){
        return;
    }
    (*num)++;
    find_num(head->next,num);
    return;
}
//求平均值
void find_add(Linklist head,int *add){
    if(head==NULL){
        return;
    }
    *add=head->data+*add;
    find_add(head->next,add);
    return ;
}
elemtype average(Linklist head){
    if(head==NULL){
        return 0;
    }
    int add=0;
    find_add(head,&add);
    int num=0;
    find_num(head,&num);
    int average_num=add/num;
    return average_num;
}
int main(){
    //这是一个头结点不存储元素的链表
    Linklist head=initlist();
    listadd(head);
    if(!head->next){
        //判定为空表
        printf("空表！不进行求值，节点数为0!");
        return 0;
    }
    int max=0;
    find_max(head->next,&max);
    printf("max:%d\n",max);
    int num=0;
    find_num(head->next,&num);
    printf("num:%d\n",num);
    int averagenum=average(head->next);
    printf("average number:%d\n",averagenum);
}