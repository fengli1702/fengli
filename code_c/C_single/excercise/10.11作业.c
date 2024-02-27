#include <stdio.h>
#include <stdlib.h>
#define row 4
#define col 4
//数组从1到m，可调用a[0],a[m],故定义为a[row+1][col+1]。a[row][col]为有定义的最后一个元素
int judge(int a[row+1][col+1]){//有意义的范围为1...row，1...col
    for(int i=1;i<=row;i++)
        for(int j=1;j<=col;j++){
            for(int p=j+1;p<=col;p++)//遍历同一行元素
                if(a[i][j]==a[i][p])
                {
                    printf("有重复（no）");
                    return 0;
                }
            for(int k=i+1;k<=row;k++){
                for(int p=1;p<=col;p++){//遍历从这一行开始到结束的元素
                    if(a[i][j]==a[k][p])
                    {
                    printf("有重复（no）");
                    return 0;
                    }
                }
            }
        }
    printf("yes");
    return 1;
}
int main(){
    int a[row+1][col+1]={{0,0,0,0,0},
                     {0,5,6,7,8},
                     {0,9,10,13,12},
                     {0,13,13,15,16},
                     {0,1,2,3,4}};//测试代码1
                     printf("测试1:");
    judge(a);
    int b[row+1][col+1]={{0,0,0,0,0},
                         {0,5,6,7,8},
                         {0,9,10,11,12},
                         {0,13,14,15,16},
                         {0,1,2,3,4}};//测试代码2
                     printf("\n测试2:");
    judge(b);
    //测试样例中0为无意义码
}
