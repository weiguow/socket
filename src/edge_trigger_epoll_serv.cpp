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

#define BUF_SIZE 1000
#define EPOLL_SIZE 20
#define SOCKET_PORT 8150

void error_handler_socket(const char* message,const int port);
void error_handler_epoll(const char* message,const int socket_port,const int epoll_fd);

int main(int argc,char* argv[])
{
    int serv_sock = 0,clnt_sock,sockfd;
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_sz;
    size_t n;
    char buf[BUF_SIZE];
    int str_len;

    int ep_fd,ep_cnt,i,flag;
    struct epoll_event event;
    struct epoll_event* pevents;
    int nPort = (argc!=2?SOCKET_PORT:atoi(argv[1]));
    
    serv_sock=socket(AF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handler_socket("socket() error",serv_sock);
    int bReuseaddr=1;
    setsockopt(serv_sock,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(int));// no wait

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(nPort);

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handler_socket("bind() error",serv_sock);

    if(listen(serv_sock,5)==-1)
        error_handler_socket("listen() error",serv_sock);

    ep_fd=epoll_create(EPOLL_SIZE);
    if(ep_fd==-1)
        error_handler_epoll("epoll_create() error",nPort,ep_fd);

    event.events=EPOLLIN | EPOLLET;
    event.data.fd=serv_sock;
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,serv_sock,&event);
    if(ep_fd==-1)
        error_handler_epoll("epoll_ctl() error",nPort,ep_fd);
    
    //pthread_t tid = 0;
    // pthread_create(&tid, NULL, EventHandle, (void*)this == 0);  
    //线程池初始化
    CThreadPoolProxy *pool = CThreadPoolProxy::instance();
    pevents=(epoll_event *)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    if(pevents!=NULL){
        while(1)
        {
            ep_cnt=epoll_wait(ep_fd,pevents,EPOLL_SIZE,10);
            for(i=0;i<ep_cnt;i++)
            {
                if(serv_sock==pevents[i].data.fd) //新接入
                {
                    clnt_addr_sz=sizeof(clnt_addr);
                    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&(clnt_addr_sz));
                    //将套接字设置成非阻塞模式
                    flag=fcntl(clnt_sock,F_GETFL,0);
                    fcntl(clnt_sock,F_SETFL,flag|O_NONBLOCK);

                    event.events=EPOLLIN|EPOLLET;
                    event.data.fd=clnt_sock;
                    epoll_ctl(ep_fd,EPOLL_CTL_ADD,clnt_sock,&event);
                    if(ep_fd==-1)
                        error_handler_epoll("epoll_ctl() error",nPort,ep_fd);
                }
                else if(pevents[i].events & EPOLLIN) { //read
                    if((sockfd = pevents[i].data.fd) < 0) continue;
                    memset(buf,0,BUF_SIZE);
                    if((n = read(sockfd, buf, BUF_SIZE)) < 0) {
                        if(errno == ECONNRESET) {
                            close(sockfd);
                            pevents[i].data.fd = -1;
                        } else {
                            printf("readline error");
                        }
                    } else if(n == 0) {
                        close(sockfd);
                        pevents[i].data.fd = -1;
                    }
                    if(strncmp(buf,"q\n",2)==0 || strncmp(buf,"Q\n",2)==0)
                        printf("client %d is closed!\n",pevents[i].data.fd);
                    else
                    {
                        // TODO:parse the buffer;
                        // TODO:mutile thred
                        // Json::Value book;
                        // book["id"] = "123";
                        // book["name"] = "android";
                        // book["author"] = "wangweiguo";
                        // book["des"] = "1";
                        // std::string body = book.toStyledString();
                        // std::cout<<book["name"]<<std::endl;
                        Json::Reader reader;
                        Json::Value data;
                        reader.parse(buf, data, false);
                        Operation* ta=new Operation;       //  具体的方法自己实现。
                        
                        ta->SetConnFd(pevents[i].data.fd);
                        ta->setOpt(Otype::ADD);
                        ta->SetConnFd(pevents[i].data.fd);
                        // std::string s = ;
                        ta->setDoing(data["name"].asString());
                        pool->AddTask(ta);
                        
                    }
                    
                    // printf("received data: %s", buf);

                    event.data.fd = sockfd;
                    event.events = EPOLLOUT | EPOLLET;
                    epoll_ctl(ep_fd, EPOLL_CTL_MOD, sockfd, &event);
                    if(ep_fd==-1)
                        error_handler_epoll("epoll_ctl() error",nPort,ep_fd);                
                }
                else if(pevents[i].events & EPOLLOUT) {  //write
                    sockfd = pevents[i].data.fd;
                    std::deque<std::pair<bool,std::string>> result = pool->getResuleQueue(sockfd);
                    string s;
                    
                    for(auto iter = result.begin();iter!=result.end();iter++){
                        s += iter->first?"success":"failed";
                        s += " ";
                        s += iter->second;
                        s += "\n";
                    }
                    write(sockfd, s.c_str(), s.size());

                    // printf("written data: %s", buf);

                    event.data.fd = sockfd;
                    event.events = EPOLLIN | EPOLLET;
                    epoll_ctl(ep_fd, EPOLL_CTL_MOD, sockfd, &event);
                    if(ep_fd==-1)
                        error_handler_epoll("epoll_ctl() error",nPort,ep_fd);
                }
            }
        }
        free(pevents);
    }
    else
    {
        fputs("malloc() error",stderr);
    }
    pool->StopAll();
    close(ep_fd);
    close(serv_sock);
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
