#    <center>lab5
#### <center>2024/5/16-
#### <center>PB22111702 李岱峰

# 一.实验准备

阅读文档：

任务一：

>write a small C program (about 200-300 lines) that simulates the behavior of a cache memory.

任务二：

> optimize a small matrix transpose function, with the goal of minimizing the number of cache misses.

根据我在计算机组成原理学到的知识，建立cache模拟器。


根据实验要求，使用LRU最近最少策略，替换最后一次访问时间最久远的那一行，使用构建时间戳的方法。

# 二.实验过程

## 1.PartA cache模拟器

参数S：高速缓存组数，E：高速缓存行数，B：每个行是由多少个字节组成的数据块数。

### 1）定义cache：
```C
typedef struct {
    int valid;//有效位
    int tag;//tag
    int lru;//LRU算法时间戳
} Line;
typedef struct cache_{
    Line **lines;//多路组相连
    int S,E,B;//组数，每组行数，块大小
} Cache;
```

如图定义。

### 2）LRU实现
```C
void update_lru(int s,int index,int tag){//
    cache->lines[s][index].tag = tag;
    cache->lines[s][index].valid = 1;
    for(int i = 0; i < cache->E; i++){
        if(cache->lines[s][i].valid){
            if(cache->lines[s][i].lru < cache->lines[s][index].lru){//时间戳小于当前时间戳的行时间戳加1
                cache->lines[s][i].lru++;
            }
        }
    }
    cache->lines[s][index].lru = 0;
}

```
如上，每次更新时，将LRU的时间更新，时间越大说明很长时间没用这个单元，更新的时候就挑这个最大LRU单元更新。

### 3）hit与miss
```C
for(int i = 0; i < cache->E; i++){
        if(cache->lines[s][i].valid && cache->lines[s][i].tag == tag){
            return 1;
        }
    }
```

如上遍历整个cache，在标记为1，tag对应的情况下，说明一次命中，否则没有命中。由于该实验没有考察hit or miss后的运行，故不写。

### 4）get_trace

获取测试内容，统计测试结果

### 5）测试结果

![配置](./src/try1.png)

如上，出问题了，不是满分。

![配置](./src/try2.png)

修复一部分问题，M型读取应该是两次access

//if(cache->lines[s][i].lru < cache->lines[s][index].lru)这行替换策略错误，该测试认为LRU中，除被访问结点外的所有有效结点的lru_time都应加1，而不是只有最新访问点后的部分结点时间加一。实测miss数确实变小了，说明所有结点都加1更有效。我考虑是由于防止时间混乱，保持整体性设计的。

![配置](./src/parta.png)