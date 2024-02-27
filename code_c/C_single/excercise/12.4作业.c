#include<stdio.h>
#include<stdlib.h>

#define Max_Vertex_Num 10
typedef struct Arcnode
{
	int adjvex;//有向边的另一个邻接点的序号 
	struct Arcnode *nextarc;//指向下一个边结点的指针		
}ArcNode;
typedef struct Vnode//顶点 
{
	char data;//顶点的信息 
	ArcNode *firstarc;
}VNode,AdjList[Max_Vertex_Num];
typedef struct
{
	AdjList vertices;//顶点数组 
	int vexnum,arcnum;//顶点数和弧的数目	
}ALGraph;//图的邻接表存储结构 
char equ[100]={'\0'};int num=0;
void PrintOutput_RPN(ALGraph *G,int i)
{
	char c;
	c=G->vertices[i].data;
    equ[num]=c;
	if(!G->vertices[i].firstarc)
		num++;
	else
	{
		ArcNode *p;
		p=G->vertices[i].firstarc;num++;
		PrintOutput_RPN(G,p->adjvex);
		PrintOutput_RPN(G,p->nextarc->adjvex);
		
	}
}
void Output_RPN(ALGraph *G)
{
	PrintOutput_RPN(G,0);
    return;
}
void Createtu(ALGraph *G)
{
	int v1,v2;
	ArcNode *e,*q;
    int vexnum,arcnum;
	printf("依次输入顶点数和弧的数目：\n");
	scanf("%d %d",&vexnum,&arcnum);
    G->arcnum=arcnum;G->vexnum=vexnum;
	for(int i=0;i<G->vexnum;i++)
	{
		getchar();
		printf("请输入第%d个元素：\n",i+1);
		G->vertices[i].data=getchar();
		G->vertices[i].firstarc=NULL;
        equ[i]=G->vertices[i].data;
	}
	for(int j=0;j<G->arcnum;j++)
	{
		printf("输入一条边的起点和终点：\n");
		scanf("%d %d",&v1,&v2);
		e=(ArcNode*)malloc(sizeof(ArcNode));
		e->adjvex=v2;
		e->nextarc=NULL;
		if(G->vertices[v1].firstarc==NULL)
			q=G->vertices[v1].firstarc=e;
		else
			q=q->nextarc=e;
	}
}
int ans[100]={0};

void find_(ALGraph *G,int w){
    ArcNode *r;
    for(r=G->vertices[w].firstarc;r;r=r->nextarc){
        find_(G,r->adjvex);
    }
    if(!r){
        num++;
        ans[num]=w;
    }
}
int main()
{
    ALGraph *G=(ALGraph *)malloc(sizeof(ALGraph)); 
	Createtu(G);
    find_(G,0);
 //   Output_RPN(G);

    for(int i=0;i<=num;i++){
       printf("%c ",equ[i]);
    }
} 


