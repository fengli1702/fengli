#include <stdio.h>
#define number 7/*the value of n in x3100*/ 
void putdown(int n);
void puton(int n);
int state[12];
int count=0;
void puton(int n){
	if(n==0) return;
	if(n==1){
		printf("\nputon the 1st ring   ");
        
        count++;
		state[11]=0;for(int i=0;i<12;i++){
        printf("%d ",state[i]);
    }
		return ;
	}
	puton(n-2);
	printf("\nputon the %dst ring   ",n);
    count++;
	state[12-n]=0;
	for(int i=0;i<12;i++){
        printf("%d ",state[i]);
    }
	putdown(n-2);
	puton(n-1);
	return ;
}
void putdown(int n){
	if(n==0) return;
	if(n==1){
		printf("\nputdown the 1st ring ");
        count++;
		state[11]=1;for(int i=0;i<12;i++){
        printf("%d ",state[i]);
    }
		return ;
	}
	putdown(n-2);
	printf("\nputdown the %dst ring ",n);
    count++;
	state[12-n]=1;
    for(int i=0;i<12;i++){
        printf("%d ",state[i]);}
	puton(n-2);
	putdown(n-1);
	return ;
}

int main(){
	for(int i=number;i>=0;i--){
        state[i]=0;
    }
	putdown(number);
//    printf("\ncount:%d",count);
	return 0;
}