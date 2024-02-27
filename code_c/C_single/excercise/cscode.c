#include <stdio.h>
#include <math.h>
long long pickGifts(int* gifts, int giftsSize, int k){
    int max=gifts[0];
    int check=0;
    for(int j=0;j<k;j++){
        max=gifts[0];
        check=0;
        for(int i=0;i<giftsSize;i++){
            if(max<gifts[i]){
                max=gifts[i];
                check=i;}}
        gifts[check]=sqrt(gifts[check]);
    }
    int all=0;
    for(int i=0;i<giftsSize;i++)
        all+=gifts[i];
    return all;
}
int main(){
    int a[]={25,64,9,4,100};
    printf("%d",pickGifts(a,5,4));
}