
/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <sys/wait.h>
#include <cctype>
#include <typeinfo>


#include "include/json/json.h"

#define BUF_SIZE 1024
#define MY_PORT 8150

using namespace std;


void error_handling(const char* message,const int sock_port);
void read_handling(const int sock,const int pid);
void write_handling(const int sock);
std::string user_operator();
void Draw_line(vector<string> vstr);
void Draw_Datas(string str);
int judge_Result(std::string &s);
bool all_is_num(string str);


sigset_t newset, zeroset;
static int counter = 1;
static int sigFlag = 0;

void sig_handler(int signo)
{
	if (signo == SIGUSR1 || signo == SIGUSR2)
	{
		sigFlag = 1;
	}
}
 
void tell_wait()
{
	sigemptyset(&newset);
	sigemptyset(&zeroset);
	sigaddset(&newset, SIGUSR1);
	sigaddset(&newset, SIGUSR2);
	
	struct sigaction action;
	action.sa_handler = sig_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	
	if (sigaction(SIGUSR1, &action, NULL) < 0)
	{
		printf("sigaction error\n");
		exit(-1);
	}
	
	if (sigaction(SIGUSR2, &action, NULL) < 0)
	{
		printf("sigaction error\n");
		exit(-1);
	}
	
	if (sigprocmask(SIG_BLOCK, &newset, NULL) < 0)
	{
		printf("sigprocmask error\n");
		exit(-1);
	}
}
 
void tell_parent(pid_t pid)
{
	kill(pid, SIGUSR2);
}
 
void wait_parent()
{
	while(sigFlag == 0)
	{
		sigsuspend(&zeroset);
	}
	
	sigFlag = 0;
	
	if (sigprocmask(SIG_BLOCK, &newset, NULL) < 0)
	{
		printf("set sigprocmask error\n");
		exit(-1);
	}
}
 
void tell_child(pid_t pid)
{
	kill(pid, SIGUSR1);
}
 
void wait_child()
{
	while(sigFlag == 0)
	{
		sigsuspend(&zeroset);
	}
	
	sigFlag = 0;
	
	if (sigprocmask(SIG_BLOCK, &newset, NULL) < 0)
	{
		printf("set sigprocmask error\n");
		exit(-1);
	}
}

int main(int argc,char* argv[])
{
    int sock;
    struct sockaddr_in addr;
    pid_t pid;
    tell_wait();
    const char *address = (argc!=3?"127.0.0.1":argv[1]);
    int nPort = (argc!=3?MY_PORT:atoi(argv[2]));

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error",sock);

    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(address);
    addr.sin_port=htons(nPort);

    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
        error_handling("connect() error",sock);

    pid=fork();
    if(pid==0)
    {
        write_handling(sock);
        
    }
    else
    {
        read_handling(sock,pid);
        
    }
    close(sock);
    return 0;
}

//写进程
void write_handling(int sock)
{
    //char buf[BUF_SIZE];
    counter = 0;
    while(1)
    {
        
        counter += 2;
        std::string body = user_operator();
        if(body == "q")
        {
            shutdown(sock,SHUT_WR);
            return;
        }
        if(!body.empty() && body!="q")
            write(sock,body.c_str(),body.size());
        tell_parent(getppid());
        wait_parent();
    }
    
}

//读进程
void read_handling(const int sock,const int pid)
{
    int str_len;
    char buf[BUF_SIZE];
    counter = -1;
    while(1)
    {
        wait_child();
        counter += 2;
        str_len=read(sock,buf,BUF_SIZE-1);
        if(str_len<=0)
            return;
        buf[str_len]=0;
        
        string result(buf);
        //result.push_back(buf);
        if(judge_Result(result)==0){
            Draw_Datas(result);
        }
        tell_child(pid);
        //printf("the message from server:%s",buf);
    }
    waitpid(pid,NULL,0);
}


void error_handling(const char* message,const int port)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    close(port);
    exit(1);
}

/*
 * 添加图书：输入 操作类型operat_type, 图书编号book_id, 书名book_name, 作者book_author, 简介book_des 输出string sbook
 * 删除图书：输入 操作类型operat_type, 图书编号book_id
 * 改动图书：输入 操作类型operat_type, 图书编号book_id
 * 查询图书：输入 操作类型operat_type, 图书编号book_id
 * */
std::string user_operator() {
    
    int operat_type;  //用户操作类型,1.增加图书 2.删除图书 3.改动图书 4.查询图书
    int book_id;       //编号
    std::string book_name;  //书名
    std::string book_author;    //作者
    std::string book_des;   //简介
    std::string sTemp;

    std::cout << "===请选择操作编号===\n"
         << "1.新增图书\n"
         << "2.删除图书\n"
         << "3.改动图书\n"
         << "4.查询图书\n"
         << "输入编号：" << std::endl;
    begin:
    std::cin >> sTemp;

    if(sTemp == "q" || sTemp == "Q")
        return "q";

    if (all_is_num(sTemp)) {
        operat_type = atoi(sTemp.c_str());
    } else {
        cout << "The input format is illegal. Please enter an integer" << endl;
        goto begin;
    }

    Json::Value book;
    std::string sbook;

    switch (operat_type) {
        case 1:{
            book["operat_type"] = 1;
            std::cout << "请输入你想添加的书名： ";
            std::cin >> book_name;
            book["name"] = book_name;
            std::cout << "请输入该书作者： ";
            std::cin >> book_author;
            book["author"] = book_author;
            std::cout << "请输入该书简介： ";
            std::cin >> book_des;
            book["des"] = book_des;
            break;
        }

            case 2 : {
                book["operat_type"] = 2;
                std::cout << "请输入图书编号: ";
                std::cin >> book_id;
                book["id"] = book_id;
                break;
            }

            case 3:  //改动图书
                int change_num;
                book["operat_type"] = 3;
                std::cout << "请输入图书编号: ";
                std::cin >> book_id;
                book["id"] = book_id;
                std::cout << "===请选择需要修改信息的项目===\n"
                        << "1.图书名称\n"
                        << "2.图书作者\n"
                        << "3.图书简介\n"
                        << "输入编号：" << std::endl;

                begin_2:

                std::cin >> sTemp;

                if (all_is_num(sTemp)) {
                    change_num = atoi(sTemp.c_str());
                } else {
                    cout << "The input format is illegal. Please enter an integer" << endl;
                    goto begin_2;
                }

                book["change_num"] = change_num;

                switch (change_num)
                {
                    case 1:
                        std::cout << "请输入你想修改的书名： ";
                        std::cin >> book_name;
                        book["name"] = book_name;

                    break;
                    case 2:
                        std::cout << "请输入修改后的作者名： ";
                        std::cin >> book_author;
                        book["author"] = book_author;

                    break;
                    case 3:
                        std::cout << "请输入新简介： ";
                        std::cin >> book_des;
                        book["des"] = book_des;

                    break;
                    default:std::cout << "The operating erro! Invalid serial number" << std::endl;break;
                }
                break;

            case 4:
                book["operat_type"] = 4;

                begin_3:
                std::cout << "请输入图书编号: ";

                std::cin >> sTemp;

                if (all_is_num(sTemp)) {
                    book_id = atoi(sTemp.c_str());
                } else {
                    cout << "The input format is illegal. Please enter an integer" << endl;
                    goto begin_3;
                }
                book["id"] = book_id;
                break;
        default:std::cout << "The operating erro!" << std::endl;sbook = "";break;
        }
    sbook = book.toStyledString();
    return sbook;
}

void Draw_line(vector<string> vstr)  //画行线
{

    for (int i = 0; i < 4; i++) {
        cout << "+-";
        for (int k = 0; k < vstr[i].size() + 1; k++) {
            cout << '-';
        }
    }cout << '+' << endl;

}

void Draw_Datas(string Str) //显示构造过程，状态转换矩阵
{
    if(Str.empty()) return;
    vector<string> header_element;
//    int size = header_element.size();
    //std::cout<<Str<<std::endl;
    header_element.push_back("book_id");
    header_element.push_back("book_name");
    header_element.push_back("book_author");
    header_element.push_back("book_des");

    Draw_line(header_element);

    for (int i = 0; i < header_element.size() ; i++) {
        cout << "| " << setw(header_element[i].size()) << setiosflags(ios::left) << setfill(' ') << header_element[i] << ' ';
    }

    cout << '|' << endl;

    Draw_line(header_element);

    Json::Reader reader;
    Json::Value data;
    reader.parse(Str, data, false);
    
    string book_id = std::to_string(data["id"].asInt());
    std::string book_name = data["name"].asString();
    string book_author = data["author"].asString();
    string book_des = data["des"].asString();

    vector<string> dbook;
    dbook.push_back(book_id);
    dbook.push_back(book_name);
    dbook.push_back(book_author);
    dbook.push_back(book_des);

    for (int i = 0; i < dbook.size(); i++) {
        cout << "| " << setw(header_element[i].size()) << setiosflags(ios::left) << setfill(' ') ;
        cout << setw(header_element[i].size())<< dbook[i] << ' ';
    }

    cout << '|'<<endl;

    Draw_line(header_element);
}


int judge_Result(string &s){
    if(s.empty()) return -1; //error
    if(s[0] == 's' || s[1] == 's'){
        s = s.substr(7);
        //std::cout<<s<<std::endl;
        return 0;
    }
    std::cerr<<s<<std::endl;
    return 1;//failed
}

bool all_is_num(string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}