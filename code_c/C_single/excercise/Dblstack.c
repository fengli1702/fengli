#include <stdio.h>
#include <stdlib.h>
//PB22111702李岱峰
#define m_size  10//数组大小
//双栈结构，数组两边都是栈，中间是栈顶
typedef int elemtype;
typedef struct {
    int top[2],bot[2];
    elemtype *v;
    int m;
}Dblstack;

Dblstack* initstack(){//初始化
    elemtype *v=(elemtype *)malloc(sizeof(elemtype)*m_size);
    Dblstack *p=(Dblstack *)malloc(sizeof(Dblstack));
    p->m=m_size;
    p->v=v;
    (p->top)[0]=(p->bot)[0]=-1;(p->top)[1]=(p->bot)[1]=p->m;
    //为了方便检验，初值附0
    for(int i=0;i<m_size;i++)
        (p->v)[i]=0;
    return p;
}
int stackempty(Dblstack *p){//判断栈空
    if((p->top)[0]==-1||(p->top)[1]==p->m)
        return 1;//空
    else
        return 0;//栈不空
}
int stack_A_length(Dblstack *p){//返回第一个栈长度
    return (p->top)[0]-(p->bot)[0];
}
int stack_B_length(Dblstack *p){//返回第二个栈长度
    return (p->bot)[1]-(p->top)[1];
}
int stackfull(Dblstack *p){//判断栈满
    int length=stack_A_length(p)+stack_B_length(p);
    if(length==p->m)
        return 1;//栈满
    else 
        return 0;//栈空
}
void L_pushstack(Dblstack *p,elemtype e){//往左压栈
    if(stackfull(p)){
        printf("栈满");
        return ;
    }
    else
        p->v[++((p->top)[0])]=e;
    return;
}
void R_pushstack(Dblstack *p,elemtype e){//往右压栈
    if(stackfull(p)){
        printf("栈满");
        return ;
    }
    else
        p->v[--((p->top)[1])]=e;
    return;
}
void L_popstack(Dblstack *p,elemtype *e){//左栈出栈
    if(stackempty(p)){
        printf("栈空");
        return;
    }
    else
        *e=p->v[((p->top)[0])--];
    return ;
}
void R_popstack(Dblstack *p,elemtype *e){//右栈出栈
    if(stackempty(p)){
        printf("栈空");
        return;
    }
    else
        *e=p->v[((p->top)[1]++)];
    return ;
}
//遍历检验
void traveral(Dblstack *p){
    for(int i=0;i<p->m;i++)
        printf("%d ",(p->v)[i]);
    return;
}
int main(){
    Dblstack *p=initstack();
    printf("\n");
    traveral(p);
    for(int i=1;i<4;i++){
        L_pushstack(p,i);
        R_pushstack(p,i);
    }
    printf("\n");
    traveral(p);
    
    int e1,e2;
    printf("\n");
    printf("top[0]=%d,top[1]=%d",(p->top)[0],(p->top)[1]);
    L_popstack(p,&e1);
    R_popstack(p,&e2);
    printf("\n");
    printf("e1=%d,top[0]=%d,e2=%d,top[1]=%d",e1,(p->top)[0],e2,(p->top)[1]);
    printf("\n");
    traveral(p);
}