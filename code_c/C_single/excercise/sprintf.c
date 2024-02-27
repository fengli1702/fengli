#include<stdio.h>
#include<string.h> 

int main()
{
  float ff=1024.58;

  char strff[21]={'-'};

  memset(strff,0,sizeof(strff));

  // 把浮点数ff转换为字符串，存放在strff中。
  sprintf(strff,"%.2f",ff);
  printf("strff=%s\n",strff);  // 输出strff=1024.58
}

