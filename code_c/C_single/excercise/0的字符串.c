#include <stdio.h>
#include <string.h>
int main(){
    char *str="1230000000";
    printf("::%s",str);
    for(int i=0;i<strlen(str);i++){
        printf("%c ",str[i]);
    }
}