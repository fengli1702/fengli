/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "haha",
    /* First member's full name */
    "deconx",
    /* First member's email address */
    "deconx@qq.com",
    /* Second member's full name (leave blank if none) */
    "feng",
    /* Second member's email address (leave blank if none) */
    "PB22111702"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 头部/脚部的大小 */
#define WSIZE 4
/* 双字 */
#define DSIZE 8

/* 扩展堆时的默认大小 */
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* 设置头部和脚部的值, 块大小+分配位 */
#define PACK(size, alloc) ((size) | (alloc))

/* 读写指针p的位置 */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) ((*(unsigned int *)(p)) = (val))

/* 从头部或脚部获取大小或分配位 */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)


/* 给定序号，找到链表第一个块位置 */
#define GET_HEAD(num) ((unsigned int *)(long)(GET(heap_list + WSIZE * num)))
/* 给定bp,找到前驱和后继 */

#define GET_PRE(bp) ((unsigned int *)(long)(GET(bp)))
#define GET_SUC(bp) ((unsigned int *)(long)(GET((unsigned int *)bp + 1)))

/* 读地址存的指针 */
#define GET_PTR(p) ((unsigned int *)(long)(GET(p)))

/* 给定有效载荷指针, 找到头部和脚部 */
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* 给定有效载荷指针, 找到前一块或下一块 */
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp) - DSIZE)))

#define CLASS_SIZE 20
/* 总是指向序言块的第二块 */
static char *heap_list;

/************************/
static void *extend_heap(size_t words);     //扩展堆
static void *coalesce(void *bp);            //合并空闲块
static void *find_fit(size_t asize);        //找到匹配的块
static void place(void *bp, size_t asize);  //分割空闲块
static void delete(void *bp);               //从相应链表中删除块
static void insert(void *bp);               //在对应链表中插入块
static int search(size_t size);             //根据块大小, 找到头节点位置
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)//初始化那张表
{
    if((heap_list = mem_sbrk((4+CLASS_SIZE) * WSIZE)) == (void *)-1)
        return -1;
    for(int i=0;i<CLASS_SIZE;i++){//初始化空闲链表
        PUT(heap_list + WSIZE * i, 0);
    }
    PUT(heap_list + WSIZE * CLASS_SIZE, 0);  //序言块头部
    PUT(heap_list + WSIZE * (CLASS_SIZE+1), PACK(DSIZE, 1));  //序言块头部
    PUT(heap_list + WSIZE * (CLASS_SIZE+2), PACK(DSIZE, 1));  //序言块脚部
    PUT(heap_list + WSIZE * (CLASS_SIZE+3), PACK(0, 1));  //结尾块
    
    if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}


/*
 * 扩展heap, 传入的是字节数
*/
void *extend_heap(size_t words)
{
    /* bp总是指向有效载荷 */
    char *bp;
    size_t size;
    /* 根据传入字节数奇偶, 考虑对齐 */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;

    /* 分配 */
    if((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    
    /* 设置头部和脚部 */
    PUT(HDRP(bp), PACK(size, 0));           /* 空闲块头 */
    PUT(FTRP(bp), PACK(size, 0));           /* 空闲块脚 */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));   /* 片的新结尾块 */

    /* 判断相邻块是否是空闲块, 进行合并 */
    return coalesce(bp);
}


/*
 * 合并空闲块
*/
void *coalesce(void *bp)
{
    //和隐式一样
    size_t prev = GET_ALLOC(FTRP(PREV_BLKP(bp)));     /* 前一块大小 */
    size_t next = GET_ALLOC(HDRP(NEXT_BLKP(bp)));     /* 后一块大小 */
    size_t size = GET_SIZE(HDRP(bp));                       /* 当前块大小 */

    if(prev && next){
        insert(bp);//前后都不空，将·块标记空闲插入空闲链表
        return bp;
    }
    /* 前不空后空 */
    else if(prev && !next){
        /* 将后面的块从其链表中删除 */
        delete(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));  //增加当前块大小
        PUT(HDRP(bp), PACK(size, 0));           //先修改头
        PUT(FTRP(bp), PACK(size, 0));           //根据头部中的大小来定位尾部
    }
    /* 前空后不空 */
    else if(!prev && next){
        /* 将其前面的快从链表中删除 */
        delete(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));  //增加当前块大小
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);                     //注意指针要变
    }
    /* 都空 */
    else{
        /* 将前后两个块都从其链表中删除 */
        delete(NEXT_BLKP(bp));
        delete(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));  //增加当前块大小
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    /* 空闲块准备好后,将其插入合适位置 */
    insert(bp);
    return bp;
}

void insert(void *bp)//得到一个块指针，将他插入到合适的链表里
{
    size_t bp_size=GET_SIZE(HDRP(bp));
    int num=search(bp_size);
    unsigned int *head=GET_HEAD(num);
    if(head==NULL){//如果是空链表
        PUT(heap_list+WSIZE*num,(unsigned int *)bp);
        PUT(bp, NULL);
        PUT((unsigned int *)bp + 1, NULL);
        //将这个块的前驱和后继都设为null
        //链表指针定向到该块
    }
    else{//如果不是空链表
    //头插法
       PUT((unsigned int *)bp+1,head);
       PUT(bp,NULL);
       PUT(head,bp);
       PUT(heap_list+num*WSIZE,bp);
    }   
}
//删除块，在需要合并时，会出现合并完块大了，换大小类的情况，所以要删除
//该块所在大小类的指针
void delete(void *bp)
{
    size_t bp_size=GET_SIZE(HDRP(bp));
    int num=search(bp_size);
    unsigned int *pre=GET_PRE(bp);
    unsigned int *suc=GET_SUC(bp);
    if(pre==NULL&&suc==NULL){//如果是唯一一个块
        PUT(heap_list+WSIZE*num,NULL);//将该大小类的指针设为null
    }
    else if(pre==NULL){//如果是头
        PUT(suc,NULL);//将后继的前驱设为NULL
        PUT(heap_list+WSIZE*num,suc);
    }
    else if(suc==NULL){//如果是尾
        PUT((unsigned int *)pre+1,NULL);//清除前驱的后继
    }
    else{//如果是中间
        PUT((unsigned int *)pre+1,suc);
        PUT(suc,pre);
    }
    PUT(bp,0);
    PUT((unsigned int *)bp+1,0);
    return;
}
/* 
 * search - 找到块大小对应的等价类的序号
 */
int search(size_t size)
{
    int i;
    for(i = 0; i <20; i++){
        if(size <= (16 << i))
            return i;
    }
    return i;

}
/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    
    size_t asize;
    size_t extendsize;
    char *bp;
    if(size == 0)
        return NULL;
    if(size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
    /* 寻找合适的空闲块 */
    if((bp = find_fit(asize)) != NULL){
        place(bp, asize);
        return bp;
    }
    /* 找不到则扩展堆 */
    extendsize = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;

}

//找到合适的块
void *find_fit(size_t asize)
{
    int num = search(asize);
    unsigned int* bp;
    /* 如果找不到合适的块，那么就搜索下一个更大的大小类 */
    while(num < CLASS_SIZE) {
        bp = GET_HEAD(num);
        while(bp) {
            if(GET_SIZE(HDRP(bp)) >= asize){
                return (void *)bp;
            }
            /* 用后继找下一块 */
            bp=GET_SUC(bp);
        }
        /* 找不到则进入下一个大小类 */
        num++;
    }
    return NULL;
}

void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    
    /* 块已分配，从空闲链表中删除 */
    delete(bp);
    if((csize - asize) >= 2*DSIZE) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        /* bp指向空闲块 */
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
        /* 加入分离出来的空闲块 */
        insert(bp);
    }
    /* 设置为填充 */
    else{
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(ptr==0)
        return;
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    /* size可能为0,则mm_malloc返回NULL */
    void *newptr;
    size_t copysize;
    
    if((newptr = mm_malloc(size))==NULL)
        return 0;
    copysize = GET_SIZE(HDRP(ptr));
    if(size < copysize)
        copysize = size;
    memcpy(newptr, ptr, copysize);
    mm_free(ptr);
    return newptr;
}