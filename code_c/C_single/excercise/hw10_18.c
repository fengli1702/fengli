#include <stdio.h>
#include <stdlib.h>
#define MAX 7       //节点数目
int find_son(int L[MAX+1],int R[MAX+1],int v,int u){
    if(L[v]==u||R[v]==u)//终止条件1，递归找到了u的祖先v
        return 1;
    else{
        if(L[v]&&find_son(L,R,L[v],u))
            return 1;//向左子树递归
        if(R[v]&&find_son(L,R,R[v],u))
            return 1;//向右子树递归
    }
    return 0;//终止条件2，没找到祖先
}
int main(){
    //树是一个满二叉树
    //                1
    //              2   3
    //            4  5 6  7
    int L[MAX+1]={0,2,4,6,0,0,0,0};
    int R[MAX+1]={0,3,5,7,0,0,0,0};
    int u,v;
    scanf("%d,%d",&u,&v);//输入u(子)，v(父)
    //请规范输入，不要越界
    if(u>MAX||v>MAX)
        printf("诶呦~请重来");
    if(find_son(L,R,v,u))
        printf("u是v子孙");
    else
        printf("u不是v子孙");
}