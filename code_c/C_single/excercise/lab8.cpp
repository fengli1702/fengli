#include <cstdint>
#include <iostream>
#include <fstream>
#include <bitset>

#define LENGTH 1
#define MAXLEN 100
#define STUDENT_ID_LAST_DIGIT 2
#define N 200 //数组长度，乒乓数用，如果要计算更多请加大N
//*//辅助函数//*/*/**///
int multi(int a,int b){//a*b
    if(a==0||b==0)
        return 0;
    int num=b-1;
    int olda=a;
    while(num>0){
        a+=olda;
        num--;
    }
    return a;
}
int subsub(int a,int b){//  a/b
    //b+b+b....>=a时返回mulb-b
    int mulb=0;
    for(int i=1;i<=a;i++){
        mulb+=b;
        if(mulb==a)
            return i;
        else if(mulb>a)
            return i-1;
    }
    return 0;
}
int subsubsub(int a,int b){//  a%b
    int num;
    num=subsub(a,b);
    int subsub=a-multi(num,b);
    return subsub;
}

///*/*/*/*/*/*/////*/*//

int  cal_if(int num){  //如果返回1则为奇数，返回0则为偶数
    if(subsubsub(num,2))
        return 1;
    return 0;
}
int odd(int num){
    //奇数处理
    int ans=0;
    int oldnum=num;
    int caltime=16;
    while(caltime>0){//printf("%d\n",subsubsub(oldnum,2));
        if(subsubsub(oldnum,2)==0)
            ans+=1;
        oldnum=subsub(oldnum,2);
        caltime--;
    }
    return ans;
}
int even(int num){
    num=65536-num;
//    printf("%d\n",num);
    int ans=0;
    int oldnum=num;
    int caltime=16;
    while(caltime>0){
        if(subsubsub(oldnum,2)==0)
            ans+=1;
        oldnum=subsub(oldnum,2);
        caltime--;
        
    }
    return ans;
}
int finailcount(int num){
    int ans=0;
    if(cal_if(num)==1)
        ans=odd(num);
    else
        ans=even(num);
    return ans;
}


int16_t lab1(int16_t n) {
    // initialize
    int16_t ans=finailcount(n);
    // calculation
    return ans+STUDENT_ID_LAST_DIGIT;
    // return value
}
int direction=1;
void judge_8(int number){
    //传入number即f(N),判断性质来决定下一个,direction=-1是向下
    int num=number;
    int check1=0,check2=0;
    while(num>0)           //and7检测是否为能被8整除的数
        num-=8;
    if(num==0)
        check1=1;
    while(number){
        int check=subsubsub(number,10);
        if(check==8)
            check2=1;
        number=subsub(number,10);
    }
    if(check1==1||check2==1){
        direction=-direction;
        return;
        }
    return;
}
int next_f(int f){
    int f_next;
 //   judge_8(f,&direction);
    judge_8(f);
    if(direction==1)
        f_next=multi(2,f)+2;
    else
        f_next=multi(2,f)-2;
    return f_next;
}
int  get_fn(int choice){
    int i=2;
    int f[N+1]={0};
    f[1]=3;
    f[2]=8;
    while(i<N){
        f[i+1]=next_f(f[i]);
        if(f[i+1]>=4096)
            f[i+1]-=4096;
//        printf("%d ",f[i]);
        i++;
    }
    return f[choice];
}
int16_t lab2(int16_t n) {
    // initialize

    // calculation
    int16_t ans=get_fn(n);
    // return value
    return ans;
}

int strcmp_my(char * str1,char *str2,int length){
    int i=0;
    for(;i<length;i++){
        if(str1[i]!=str2[i])
            return 0;
    }
    return 1;
}

int16_t lab3(char s1[], char s2[], int input_cnt, char my_input[10][MAXLEN]) {
    if(strcmp_my(s1,my_input[0],5)){
        printf("righ");
        return 0;
    }
    else{
        printf("wron");
        printf("\n%s",s2);
        int i=0;
        while(i<input_cnt){
            i++;
            if(strcmp_my(s2,my_input[i],5)){
                if(strcmp_my(s1,my_input[i+1],5)){
                printf("\nrigh");
                }
                else{
                    printf("\nwron");
                }
            return 0;
        }
        else{
            printf("\nwron");
            return 0;
        }
        }
    }  return 0;
}



void putdown(int n,int memory[MAXLEN][16]);
void puton(int n,int memory[MAXLEN][16]);
int state[12];
int count=0;
void puton(int n,int memory[MAXLEN][16]){
	if(n==0) return;
	if(n==1){
//		printf("\nputon the 1st ring   ");
        count++;
		state[11]=0;
        for(int i=0;i<12;i++){
            memory[count][i+4]=state[i];
    }

		return ;
	}
	puton(n-2,memory);
//	printf("\nputon the %dst ring   ",n);
    count++;
	state[12-n]=0;
	for(int i=0;i<12;i++){
        memory[count][i+4]=state[i];
    }
	putdown(n-2,memory);
	puton(n-1,memory);
	return ;
}
void putdown(int n,int memory[MAXLEN][16]){
	if(n==0) return;
	if(n==1){
//		printf("\nputdown the 1st ring ");
        count++;
		state[11]=1;for(int i=0;i<12;i++){
        memory[count][i+4]=state[i];
    }
		return ;
	}
	putdown(n-2,memory);
//	printf("\nputdown the %dst ring ",n);
    count++;
	state[12-n]=1;
    for(int i=0;i<12;i++){
        memory[count][i+4]=state[i];}
	puton(n-2,memory);
	putdown(n-1,memory);
	return ;
}

int lab4(int memory[MAXLEN][16], int n) {
    // initialize
    count=0;
    for(int j = 0; j < MAXLEN; j++){
            for(int i=0;i<16;i++)
            memory[j][i]=0 ;
        }
    // calculation
    for(int i=11;i>=0;i--){
        state[i]=0;
    }
	putdown(n,memory);

	return count;
    // return value
}

int main() {
    std::fstream file;
    file.open("test.txt", std::ios::in);

    // lab1
    int16_t n = 0;
    std::cout << "===== lab1 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab1(n) << std::endl;
    }

    // lab2
    std::cout << "===== lab2 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab2(n) << std::endl;
    }

   

    // lab3
    std::cout << "===== lab3 =====" << std::endl;
    char passwd[MAXLEN]; char verify[MAXLEN];
    int input_cnt=-1;
    char my_input[10][MAXLEN];
    for (int i = 0; i < LENGTH; ++i) {
        file >> passwd >> verify;
        file >> input_cnt;
        for (int j=0; j< input_cnt; j++)
        {
            file >> my_input[j];
        }
        lab3(passwd, verify , input_cnt, my_input);
        std::cout<<std::endl;
    }

    // lab4
    std::cout << "===== lab4 =====" << std::endl;
    int  move;
    int memory[MAXLEN][16]={0};
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        move = lab4(memory, n);
        for(int j = 1; j <= move; ++j){
            for(int i=0;i<16;i++)
            std::cout << memory[j][i];
            std::cout<<std::endl;
        }
    }
    
    file.close();
    return 0;
}