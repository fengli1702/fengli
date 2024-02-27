#include<stdio.h>

#define row 4
#define col 4
 //测试时使用的是m,n都为4
int find_x[row][col]={0};//找行里最小的，用1标记；
int find_y[row][col]={0};//找到的列最大坐标，用1标记；
int a[row][col]={{1,2,3,4},
                {5,6,7,8},
                {9,10,11,12},
                {13,13,15,16}};//测试代码
void min_find(){
    for(int i=0;i<row;i++){//遍历,i行
        int min=a[i][0];
        for(int j=0;j<col;j++){
            if(a[i][j]<min)
                min=a[i][j];
        }//得到每一行最小值
        for(int j=0;j<col;j++)
            if(a[i][j]==min)
                find_x[i][j]=1;
    }
}
void max_find(){
    for(int j=0;j<col;j++){//遍历,j列
        int max=a[0][j];
        for(int i=0;i<row;i++){
            if(a[i][j]>max)
                max=a[i][j];
        }//得到每一列最小值
        for(int i=0;i<row;i++)
            if(a[i][j]==max)
                find_y[i][j]=1;
    }
}
//find_x和find_y中都为1的点，为马点
int main(){
    min_find();
    max_find();
    int find=0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(find_x[i][j]==1&&find_y[i][j]==1){
                printf("point:(%d,%d)=%d\n",i,j,a[i][j]);
                find++;
            }
        }
    }
    if(find==0)
        printf("没找到");
    else{
        printf("找到%d个",find);
    }
}