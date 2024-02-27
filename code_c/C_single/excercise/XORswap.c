#include <stdio.h>
void swap(int *a,int *b){
    int c=*a^(*b);
    *a=c^(*a);
    *b=c^(*b);
}
void sort(int *a, int n) {
// sort a[0] ~ a[n - 1]
for (int i = 0; i < n - 1; i++) {
int min = i;
for (int j = i; j < n; j++) {
if (a[j] < a[min]) {
min = j;
}
}
swap(a + i, a + min);
}
}
int b[12]={1,2,3,4,5,6,7,8,9,10,11,12};
int main(){
    sort(b,12);
    for(int i=0;i<12;i++)
    printf("%d ",b[i]);
}
