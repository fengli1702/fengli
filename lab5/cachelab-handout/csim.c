#include "cachelab.h"
//对该文件进行修改 PartA
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

//定义cache
typedef struct {
    int valid;//有效位
    int tag;//tag
    int lru;//LRU算法时间戳
} Line;
typedef struct cache_{
    Line **lines;//多路组相连
    int S,E,B;//组数，每组行数，块大小
} Cache;

Cache *cache=NULL;
int hit_count=0,miss_count=0,eviction_count=0;
int verbose=0;//是否输出详细信息


void init_cache(int s,int e,int b){
    cache=(Cache *)malloc(sizeof(Cache));
    cache->S =1<< s;
    cache->E = e;
    cache->B = 1<<b;
    cache->lines = (Line **)malloc(sizeof(Line *) * cache->S);
    for(int i = 0; i < cache->S; i++){
        cache->lines[i] = (Line *)malloc(sizeof(Line) * cache->E);
        for(int j = 0; j < cache->E; j++){
            cache->lines[i][j].valid = 0;
            cache->lines[i][j].tag = -1;
            cache->lines[i][j].lru = 0;
        }
    }
}
//时间戳每次更新时都要变化，时间戳越大表示访问时间越远
void update_lru(int s,int index,int tag){//
    cache->lines[s][index].tag = tag;
    cache->lines[s][index].valid = 1;
    for(int i = 0; i < cache->E; i++){
        if(cache->lines[s][i].valid){
            //if(cache->lines[s][i].lru < cache->lines[s][index].lru){//时间戳小于当前时间戳的行时间戳加1
                cache->lines[s][i].lru++;
            //}
        }
    }
    cache->lines[s][index].lru = 0;
}
//找到最大时间戳替换
int find_max_lru(int s){
    int max = 0;
    int index = 0;
    for(int i = 0; i < cache->E; i++){
        if(cache->lines[s][i].lru > max){
            max = cache->lines[s][i].lru;
            index = i;
        }
    }
    return index;
}
//查找是否命中
int is_hit(int s,int tag){
    for(int i = 0; i < cache->E; i++){
        if(cache->lines[s][i].valid && cache->lines[s][i].tag == tag){
            return 1;
        }
    }
    return 0;
}
//访问
void access_cache(int s,int tag){
    if(is_hit(s,tag)){
        hit_count++;
        if(verbose){
            printf("hit ");
        }
        //更新LRU
        for(int i = 0; i < cache->E; i++){
            if(cache->lines[s][i].valid && cache->lines[s][i].tag == tag){
                update_lru(s,i,tag);
                break;
            }
        }
    }else{
        miss_count++;
        if(verbose){
            printf("miss ");
        }
        int index = -1;
        for(int i = 0; i < cache->E; i++){
            if(!cache->lines[s][i].valid){
                index = i;
                break;
            }
        }
        if(index == -1){//cahce is  full
            eviction_count++;
            if(verbose){
                printf("eviction ");
            }
            index = find_max_lru(s);
        }
        update_lru(s,index,tag);
    }
}
//释放cache
void free_cache(){
    for(int i = 0; i < cache->S; i++){
        free(cache->lines[i]);
    }
    free(cache->lines);
    free(cache);
}
//get_trace
char t[1000];
void get_trace(int s,int e,int b){
    FILE *file = fopen(t,"r");
    if(file == NULL){
        printf("open file error\n");
        exit(1);
    }
    char buf[1000];
    char type;
    unsigned long address;
    int size;
    while(fgets(buf,1000,file) != NULL){
        sscanf(buf," %c %lx,%d",&type,&address,&size);
        if(type == 'I'){
            continue;
        }
        if(verbose){
            printf("%c %lx,%d ",type,address,size);
        }
        int tag = address >> (s + b);
        int index = (address >> b) & ((unsigned)(-1) >> (8 * sizeof(unsigned) - s));
        if(type == 'L' || type == 'S'){
            access_cache(index,tag);
        }
        else if(type == 'M'){
            access_cache(index,tag);
            access_cache(index,tag);
        }
        
        if(verbose){
            printf("\n");
        }
    }
    fclose(file);
}
//help
void print_help(){
    printf("** A Cache Simulator by Deconx\n");
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("-h         Print this help message.\n");
    printf("-v         Optional verbose flag.\n");
    printf("-s <num>   Number of set index bits.\n");
    printf("-E <num>   Number of lines per set.\n");
    printf("-b <num>   Number of block offset bits.\n");
    printf("-t <file>  Trace file.\n\n\n");
    printf("Examples:\n");
    printf("linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

int main(int agrc,char* argv[])
{
    char opt;
    int s,e,b;
    while((opt = getopt(agrc,argv,"hvs:E:b:t:")) != -1){
        switch(opt){
            case 'h':
                print_help();
                return 0;
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                e = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                strcpy(t,optarg);
                break;
            default:
                print_help();
                return 1;
        }
    }
    init_cache(s,e,b);
    get_trace(s,e,b);
    free_cache();
    printSummary(hit_count,miss_count,eviction_count);
    return 0;
}
