#include <stdio.h>
#include <stdlib.h>
#define MAX 20
//深度每多一层，满二叉树中的右分支(1)的权重就乘2，发现满二叉树中(根结点为某右节点)，树的
//数列表示中，[1...n]对应的就是其10进制数，如第一个(根结点)，在数组中为1，十进制数为1
//第i个点，对应的十进制数为i
//满二叉树最左边一列对应1，2，4，8...，正好是第1，10，100，1000(二进制)个点，其兄弟结点正好为1，11，101，1001...
//由此：
int find_10(int i){
    return i+1;//设结点从0开始存
}
int main(){
    int tree[MAX]={0,1,2,3,4,5,6,7,8,9};//树的例子
    printf("输入0-9：");
    int i=0;
    scanf("%d",&i);//根结点为第0位
    printf("\nfind_10:%d",find_10(i));
}