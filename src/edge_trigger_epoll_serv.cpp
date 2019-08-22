/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <deque>
#include <iostream>
#include "Operation.h"
#include "Thread.h"
#include "json/json.h"
#include "CTimer.h"

#define BUF_SIZE 1000
#define EPOLL_SIZE 20
#define SOCKET_PORT 8150

void error_handler_socket(const char* message,const int port);
void error_handler_epoll(const char* message,const int socket_port,const int epoll_fd);
void read_data_file();
void write_data_file();
//void send_socket(epoll_event &events,epoll_event &event2,);
int socket_bind(const int port)
{
    int  listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (listenfd == -1)
    {
        error_handler_socket("socket() error",listenfd);
    }
    bzero(&servaddr,sizeof(servaddr));
    int bReuseaddr=1;
    setsockopt(listenfd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(int));// no wait

    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
       error_handler_socket("bind() error",listenfd);

    }
    return listenfd;
}

void add_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
    if(epollfd==-1)
        error_handler_epoll("epoll_ctl() error",fd,epollfd);
}

void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}
void handle_accpet(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen;
    clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if (clifd == -1)
        perror("accpet error:");
    else
    {
        //将套接字设置成非阻塞模式
        int flag=fcntl(clifd,F_GETFL,0);
        fcntl(clifd,F_SETFL,flag|O_NONBLOCK);
        printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        //添加一个客户描述符和事件
        add_event(epollfd,clifd,EPOLLIN);
    }
}

void do_read(int epollfd,int fd,char *buf)
{
    int nread;
    memset(buf,0,BUF_SIZE);
    nread = read(fd,buf,BUF_SIZE);
    if (nread == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else if (nread == 0)
    {
        fprintf(stderr,"client close.\n");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else
    {
        Json::Reader reader;
        Json::Value data;
        reader.parse(buf, data, false);
        Operation* ta=new Operation;
        
        OTYPE opt = OTYPE(data["operat_type"].asInt()-1);
        ta->setOpt(opt);
        if(opt == OTYPE::MODIFY) //modify
            ta->setModifyType(MTYPE(data["change_num"].asInt()-1));
        ta->SetConnFd(fd);
        ta->setData(data);
        CThreadPoolProxy::instance()->AddTask(ta);
        //修改描述符对应的事件，由读改为写
        modify_event(epollfd,fd,EPOLLOUT);
    }

}

static void do_write(int epollfd,int fd,char *buf)
{
    int nwrite;
    string s = "";
    while(s.empty())
        s = CThreadPoolProxy::instance()->getResult(fd);
    //printf("come in write\n");
    if(!s.empty()){
        nwrite = write(fd, s.c_str(), s.size()+1);
    }
    else{
        printf("server error\n");
    }
    if (nwrite == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLOUT);
    }
    else
        modify_event(epollfd,fd,EPOLLIN);
}

void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf)
{
    int i;
    int fd;
    //进行选好遍历
    for (i = 0;i < num;i++)
    {
        fd = events[i].data.fd;
        //根据描述符的类型和事件类型进行处理
        if ((fd == listenfd) &&(events[i].events & EPOLLIN))
            handle_accpet(epollfd,listenfd);
        else if (events[i].events & EPOLLIN)
            do_read(epollfd,fd,buf);
        else if (events[i].events & EPOLLOUT)
            do_write(epollfd,fd,buf);
    }
}

void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event* pevents;
    int ret;
    char buf[BUF_SIZE];
    memset(buf,0,BUF_SIZE);
    //创建一个描述符
    epollfd=epoll_create(EPOLL_SIZE);
    if(epollfd==-1)
        error_handler_epoll("epoll_create() error",listenfd,epollfd);
    
    pevents=(epoll_event *)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    if(pevents == NULL){
        printf("malloc error()");
        exit(1);
    }
    //添加监听描述符事件
    add_event(epollfd,listenfd,EPOLLIN);
    while(1)
    {
        //获取已经准备好的描述符事件
        ret = epoll_wait(epollfd,pevents,EPOLL_SIZE,-1);
        handle_events(epollfd,pevents,ret,listenfd,buf);
    }
    close(epollfd);
    close(listenfd);
}

int main(int argc,char* argv[])
{
    int serv_sock = 0;
    int nPort = (argc!=2?SOCKET_PORT:atoi(argv[1]));
    serv_sock = socket_bind(nPort);
    read_data_file();
    CTimer *pTimer = new CTimer("timer 1");
	pTimer->AsyncLoop(5*1000, write_data_file);
    if(listen(serv_sock,5)==-1)
        error_handler_socket("listen() error",serv_sock);

    do_epoll(serv_sock);

    return 0;
}
void error_handler_socket(const char* message,const int port)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    if(port!=-1)
        close(port);
    exit(1);
}


void error_handler_epoll(const char* message,const int socket_port,const int epoll_fd)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    if(socket_port!=-1)
        close(socket_port);
    if(epoll_fd!=-1)
        close(epoll_fd);
    exit(1);
}


void read_data_file(){
    vector<std::string> db;
    if(readfile(FILE_PATH,db)!=0){
        printf("read file error\n");
        return;
    }
    Book b;
    Json::Reader reader;
    Json::Value jData;
    for(string s:db){
        reader.parse(s, jData, false);
        b.setBookId(jData["id"].asInt());
        b.setsBookName(jData["name"].asString());
        b.setsAuthor(jData["author"].asString());
        b.setsBookDes(jData["des"].asString());
        Map::instance()->insertMap(b);
    }
}

void write_data_file(){
    vector<string> db;
    //printf("test: 1\n");
    for(std::pair<int,Book> b : Map::instance()->getMap()){
        db.push_back((b.second).to_string()+"\n");
    }
    //printf("test: 2\n");
    if(writefile(FILE_PATH,db)!=0){
        printf("save the data error\n");
    }
}