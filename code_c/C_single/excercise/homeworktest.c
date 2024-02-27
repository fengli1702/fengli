#include <stdio.h>
void delete(int a[],int item,int length){
    int find=0;
    for(int i=0;i<length;i++){
        if(a[i]==item){
            find++;
            continue;
        }
        else if(find!=0){
            a[i-find]=a[i];
        }
    }
    length-=find;
    printf("%d",length);
}
int main(){
  int a[12]={0,1,2,3,4,5,5,5,6,8,9,5};
  delete(a,5,12);
  for(int i=0;i<=8;i++)
    printf("%d ",a[i]);
}