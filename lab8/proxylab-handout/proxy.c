
#include "csapp.h"
#include "sbuf.c"
#include "cache.c"
/* Recommended max cache and object sizes */

#define SBUFSIZE 16
#define NTHREADS 4
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
//全局队列
sbuf_t sbuf;
//
typedef struct {
    char *name;
    char *object;
} CacheLine;

typedef struct {
    int used_cnt;
    CacheLine* objects;
} Cache;

//生成全局cache
Cache cache;
int readcnt; //用来记录读者的个数
sem_t mutex, w; //mutex用来给readcnt加锁，w用来给写操作加锁
void init_cache() {
    Sem_init(&mutex, 0, 1);
    Sem_init(&w, 0, 1);
    readcnt = 0;
    cache.objects = (CacheLine*)Malloc(sizeof(CacheLine) * 10);
    cache.used_cnt = 0;
    for (int i = 0; i < 10; ++i) {
        cache.objects[i].name = Malloc(sizeof(char) * MAXLINE);
        cache.objects[i].object = Malloc(sizeof(char) * MAX_OBJECT_SIZE);
    }
}
int reader(int fd, char *uri) {
    int in_cache= 0;
    P(&mutex);
    readcnt++;
    if (readcnt == 1) {
        P(&w);
    }
    V(&mutex);

    for (int i = 0; i < 10; ++i) {
        if (!strcmp(cache.objects[i].name, uri)) {
            Rio_writen(fd, cache.objects[i].object, MAX_OBJECT_SIZE);
            in_cache = 1;
            break;
        }
    }
    P(&mutex);
    readcnt--;
    if (readcnt == 0) {
        V(&w);
    }
    V(&mutex);
    return in_cache;
}

void writer(char *uri, char *buf) {
    P(&w);
    strcpy(cache.objects[cache.used_cnt].name, uri);
    strcpy(cache.objects[cache.used_cnt].object, buf);
    ++cache.used_cnt;
    V(&w);
}
//
/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
void todo(int fd);
void *thread(void *vargp);
void parse_uri(char *uri, char *hostname, char *pathname, char *port);
void build_header(char *server, char *hostname, char *pathname, char *port, rio_t *client_rio);
int main(int argc, char **argv)
{

    init_cache();
    int listenfd, connfd;
    socklen_t clientlen;
    char hostname[MAXLINE], port[MAXLINE];
    struct sockaddr_storage clientaddr;
  
    if (argc != 2)
    {
        fprintf(stderr, "usage :%s <port> \n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);

    pthread_t tid;
    sbuf_init(&sbuf, SBUFSIZE);//预线程

     for(int i = 0; i < NTHREADS; i++)
    {
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        sbuf_insert(&sbuf, connfd);//插入连接描述符

        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s %s).\n", hostname, port);

        //关闭客户端的连接描述符
        //Close(connfd);
    }
    return 0;
}


void todo(int fd){//转发:从server转发给client，从client转发给server

    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t client_rio,server_rio;
    char server[MAXLINE];

    Rio_readinitb(&client_rio, fd);//将client端写口fd导入client_rio
    Rio_readlineb(&client_rio, buf, MAXLINE);

    sscanf(buf, "%s %s %s", method, uri, version);
    
    if (strcasecmp(method, "GET"))
    {
        printf("Proxy does not implement the method");
        return;
    }
    //-------------------cache
    char cache_tag[MAXLINE];
    strcpy(cache_tag, uri);

    if(reader(fd, cache_tag))
    {
        return;
    }

    //没命中往下走
    //----------------------
    char hostname[MAXLINE], pathname[MAXLINE], port[MAXLINE];
    parse_uri(uri, hostname, pathname, port);
    build_header(server, hostname, pathname, port, &client_rio);

    int serverfd = Open_clientfd(hostname, port);//连接服务器
   
    Rio_readinitb(&server_rio, serverfd);
    Rio_writen(serverfd, server, strlen(server));

    size_t n;
    int size_buf=0;
    char cache_buf[MAX_OBJECT_SIZE];

    while ((n = Rio_readlineb(&server_rio, buf, MAXLINE)) != 0)//收到web信息转发回复client
    {
        printf("proxy received %ld bytes, then send\n", n);
        size_buf+=n;
        if(size_buf<MAX_OBJECT_SIZE){
            buf[n] = '\0';
            strcat(cache_buf,buf);
        }
        Rio_writen(fd, buf, n);
    }
    Close(serverfd);

    if(size_buf<MAX_OBJECT_SIZE){
        writer(cache_tag, cache_buf);
    }
}

void parse_uri(char *uri, char *hostname, char *pathname, char *port)
{
    char *pos = strstr(uri, "//");
    pos = pos ? pos + 2 : uri;
    char *pos2 = strstr(pos, ":");
    if (pos2)//标记了端口号
    {
        *pos2 = '\0';
        sscanf(pos, "%s", hostname);
        pos = pos2 + 1;
        pos2 = strstr(pos, "/");
        *pos2 = '\0';
        sscanf(pos, "%s", port);
        *pos2 = '/';
        sscanf(pos2, "%s", pathname);
    }
    else//没标记端口号默认80
    {
        pos2 = strstr(pos, "/");
        *pos2 = '\0';
        sscanf(pos, "%s", hostname);
        *pos2 = '/';
        sscanf(pos2, "%s", pathname);
        strcpy(port, "80");
    }
}

void build_header(char *server, char *hostname, char *pathname, char *port, rio_t *client_rio)
{
    char buf[MAXLINE], request[MAXLINE];
    sprintf(request, "GET %s HTTP/1.0\r\n", pathname);
    while (Rio_readlineb(client_rio, buf, MAXLINE) > 0)
    {
        if (!strcmp(buf, "\r\n"))
            break;
        if (!strncasecmp(buf, "Host", 4))
            continue;
        if (!strncasecmp(buf, "Connection", 10))
            continue;
        if (!strncasecmp(buf, "Proxy-Connection", 16))
            continue;
        if (!strncasecmp(buf, "User-Agent", 10))
            continue;
        if (!strncasecmp(buf, "Accept", 6))
            continue;
        if (!strncasecmp(buf, "Accept-Encoding", 15))
            continue;
        if (!strncasecmp(buf, "Accept-Language", 15))
            continue;
        strcat(request, buf);
    }
    sprintf(server, "%sHost: %s\r\n%s%s%s%s%s%s%s", request, hostname, user_agent_hdr, "Connection: close\r\n", "Proxy-Connection: close\r\n", "\r\n", "\r\n", "\r\n", "\r\n");
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1)
    {
        int connfd = sbuf_remove(&sbuf);
        todo(connfd);
        Close(connfd);
    }
    return NULL;
}