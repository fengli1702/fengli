//逆波兰表达式计算
#include <stdio.h>
#include <stdlib.h>
#define Max 20
typedef double elemtype;
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
elemtype pop_stack(Seqstack *s){
    //出栈
    if(emptystack(s))
        return -1;
    else
        return (s->data[(s->top)--]);
}
elemtype top_stack(Seqstack *s){
    //取栈顶
    if(emptystack(s))
        return -1;
    else
        return (s->data[(s->top)]);
}
void calculate(){
    char x;
    Seqstack *num_stack=InitStack();
    scanf("%c",&x);
    float op1,op2;
    float num=0.0;
    while(x!='$'){
        switch(x){//挑选表达式中的值
            case '+':if(!emptystack(num_stack)){
                    op1=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    if(!emptystack(num_stack)){
                    op2=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    push_stack(num_stack,op1+op2);
                    scanf("%c",&x);
                    break;
            case '-':if(!emptystack(num_stack)){
                    op1=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    if(!emptystack(num_stack)){
                    op2=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    push_stack(num_stack,op1-op2);
                    scanf("%c",&x);
                    break;
            case '*':if(!emptystack(num_stack)){
                    op1=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    if(!emptystack(num_stack)){
                    op2=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    push_stack(num_stack,op1*op2);
                    scanf("%c",&x);
                    break;
            case '/':if(!emptystack(num_stack)){
                    op1=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    if(!emptystack(num_stack)){
                    op2=top_stack(num_stack);
                    pop_stack(num_stack);
                    }
                    push_stack(num_stack,op1/op2);
                    scanf("%c",&x);
                    break;
            case ' ': scanf("%c",&x);  break;
            default:
                 if((x>='0'&&x<='9')){
                    while((x>='0'&&x<='9')){
                            //不为空时，要么数字为个位数，直接压栈，要么不为个位数，需要计算
                            num=num*10+(x-'0');
                            scanf("%c",&x);  
                    }
                    if(x=='.'){//遇到小数了
                        double scale=10.0;
                        scanf("%c",&x);//再读一个数
                        while((x>='0'&&x<='9')){
                            num=num+(x-'0')*1.0/scale;
                            scale=scale*10;
                            scanf("%c",&x); }
                    }
                    push_stack(num_stack,num);num=0;
                  }
                 else 
                     printf("error ");
            }
    }
    float num_finial=top_stack(num_stack);
    printf("finial number:%.3lf",num_finial);
}
int main(){
    calculate();
    return 1;
}