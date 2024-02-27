#include <stdio.h>
#include <stdlib.h>
//二叉树
//链式
typedef int elemtype;
typedef struct BiTNode{
    elemtype data;
    struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
typedef struct{
    BiTree node;
    int depth;
}BiTreedepth;
//初始化
BiTree  initBiTree(){
    BiTree T=(BiTree )malloc(sizeof(BiTNode));//根结点
    T->data=0;
    T->lchild=NULL;
    T->rchild=NULL;
    T->depth=1;
    return T;
}
//销毁二叉树
void destroyBiTree(BiTree T){
    //后序遍历毁树
    if(T){
        destroyBiTree(T->lchild);
        destroyBiTree(T->rchild);
        free(T);
    }
    else 
        return;
}
//树是否空
void BiTreeempty(BiTree T){
    if(T)
        return 1;
    else 
        return 0;
}
//返回深度
BiTreedepth find_deep(BiTree T){
    if(T==NULL)
        return (BiTreedepth){NULL,0};
    BiTreedepth left=find_deep(T->lchild);
    BiTreedepth right=find_deep(T->rchild);
    if(left.depth<right.depth)
        return (BiTreedepth){right.node,right.depth+1};
    if(left.depth>right.depth)
        return (BiTreedepth){left.node,left.depth+1};
    return 
        (BiTreedepth){left.node,left.depth+1};
}
int BiTree_finddeep(BiTree T){
    int deep=find_deep(T).depth;
    return deep;
}
//插入结点
void insertBiTree(BiTree T){
    
}
//递归遍历
void InorderTraverse(BiTree T){//中序
    if(T){
        InorderTraverse(T->lchild);
        printf("%d",T->data);
        InorderTraverse(T->rchild);
    }
    else
        return;
}
//非递归中序遍历
