#include <stdio.h>
#include <stdlib.h>
#define Max 20
typedef int elemtype;
typedef struct {
    elemtype data[Max];
    int top;
}Seqstack;
Seqstack * InitStack(){
    Seqstack *s=(Seqstack *)malloc(sizeof(Seqstack));
    s->top=-1;
    return s;
}
int emptystack(Seqstack *s){
    if(s->top==-1)
        return 1;//空
    else  
        return 0;//非空
}
int push_stack(Seqstack *s,elemtype data){
    //压栈
    if(s->top!=Max){
        s->data[++(s->top)]=data;
        return 1;}
    else{
        printf("error");
        return 0;}
}
int pop_stack(Seqstack *s){
    //出栈
    if(emptystack(s))
        return -1;
    else
        return (s->data[(s->top)--]);
}
int top_stack(Seqstack *s){
    //取栈顶
    if(emptystack(s))
        return -1;
    else
        return (s->data[(s->top)]);
}
void traverse_stack(Seqstack *s){
    //从栈顶遍历到栈底
    if(emptystack(s))
        return ;
    else{
        int size=s->top;
        while(size!=-1)
        printf("%d ",s->data[size--]);
        return;
    }
}
int main(){
    Seqstack *s=InitStack();
    printf("if kong:%d\n",emptystack(s));
    for(int i=0;i<10;i++)
        push_stack(s,i);
    traverse_stack(s);
    int change=pop_stack(s);
    printf("\nchange_top:%d\n",change);
    int data=top_stack(s);
    printf("top=%d\n",data);
    traverse_stack(s);
}