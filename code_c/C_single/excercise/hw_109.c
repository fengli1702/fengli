 #include<stdio.h>
 #define MAX 20
 #define row 4
 #define col 4
 //测试时使用的是m,n都为4,
 typedef struct{
    int x[MAX];
    int y[MAX];
    int num;//定义了有几个值
}Node;//定义结构体数组储存，第一个元素存的是马鞍点的个数
int main(){
    Node finded[MAX];
    int a[row][col]={{1,2,3,4},
                    {5,6,7,8},
                    {9,10,11,12},
                    {13,14,15,16}};
    if (find(a,finded)){//调用函数寻找
        printf ("找到!\n ");
        printf("共%d个",finded[0].num);}
    else 
    printf ("无");
    return 0;
}
//找出矩阵中每行的最小值
void Min_find( int a[row][col],Node min[MAX]){
    int j=0;
    min[0].num=0;
    for(j=0;j<row;j++){//遍历,j行
        min[j+1].num=a[j][0];
        for(int i=0;i<col;i++){
            if(a[j][i]<min[j+1].num){
                min[j+1].num=a[j][i];
                min[j+1].x[0]=j;
                min[j+1].y[0]=i;
                min[j+1].num=1;}
    }//得到每一行最小值
    }
    for(int k=0;k<row;k++){//考虑重复情况
        for(int i=0;i<col;i++){
            if(a[k][i]==min[k+1].num&&i!=min[k+1].y[0]){
                min[k+1].x[min[k+1].num]=k;
                min[k+1].y[min[k+1].num]=i;
                min[k+1].num+=1;}
    }//包括重复，所有点都在min中
    }
}
//找每一列最大值
void Max_find( int a[row][col],Node max[MAX]){
    int j=0;
    max[0].num=0;
    for(j=0;j<row;j++){//遍历,j行
        max[j+1].num=a[j][0];
        for(int i=0;i<col;i++){
            if(a[j][i]>max[j+1].num){
                max[j+1].num=a[j][i];
                max[j+1].x[0]=j;
                max[j+1].y[0]=i;
                max[j+1].num=1;}
    }//得到每一行最小值
    }
    for(int k=0;k<row;k++){//考虑重复情况
        for(int i=0;i<col;i++){
            if(a[k][i]==max[k+1].num&&i!=max[k+1].y[0]){
                max[k+1].x[max[k+1].num]=k;
                max[k+1].y[max[k+1].num]=i;
                max[k+1].num+=1;}
    }//包括重复，所有点都在min中
    }
}
int find(int a[row][col],Node finded[MAX]){//找马点
    int i,k;
    Node min[MAX];
    Node max[MAX];
    Min_find(a,min);
    Max_find(a,max);
    //min中有行最小值，max中有列最大值，如果两个坐标相等，那就是马鞍点
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(min[i].y[]=)
        }
    }

}
