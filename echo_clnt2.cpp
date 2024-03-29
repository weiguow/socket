
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
void Draw_Datas(std::string str);
int judge_input(std::string s);
int judge_Result(std::string &s);
bool all_is_num(std::string str);


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
        while(body=="error")
            body = user_operator();
        if(body == "q")
        {
            shutdown(sock,SHUT_WR);
            tell_parent(getppid());
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

    std::string sTemp;      //一级操作编号

    std::cout << "Please select operation num\n"
         << "1.Add Book\n"
         << "2.Delete Book\n"
         << "3.Modify Book\n"
         << "4.Search Book\n"
         << "Exit q\n"
         << "Input operation num:";

    cin >> sTemp;

    operat_type = judge_input(sTemp);

    if (operat_type == -1) {
        return "q";
    }

    Json::Value book;
    std::string sbook;

    switch (operat_type) {
        case 1:{
            book["operat_type"] = 1;
            std::cout << "book name:";
            std::cin >> book_name;
            book["name"] = book_name;
            std::cout << "book author:";
            std::cin >> book_author;
            book["author"] = book_author;
            std::cout << "book des:";
            std::cin >> book_des;
            book["des"] = book_des;
            break;
        }

        case 2 : {
            book["operat_type"] = 2;
            cout << "Please input book id:";
            cin >> sTemp;
            book_id = judge_input(sTemp);
            if (book_id == -1) {
                return "q";
            }
            book["id"] = book_id;
            break;
        }

        case 3:  //改动图书
            book["operat_type"] = 3;
            cout << "Please input book id:";
            cin >> sTemp;
            book_id = judge_input(sTemp);

            if (book_id == -1) {
                return "q";
            }

            book["id"] = book_id;

            std::cout << "Please select type you want change\n"
                    << "1.Book Name\n"
                    << "2.Book Author\n"
                    << "3.Book Description\n"
                    << "Exit q\n"
                    << "Input operation num:";

            int change_num;
            cin >> sTemp;
            change_num = judge_input(sTemp);

            if (change_num == -1) {
                return "q";
            }
            book["change_num"] = change_num;

            switch (change_num)
            {
                case 1:
                    std::cout << "Enter the book name you want to change:";
                    std::cin >> book_name;
                    book["name"] = book_name;

                break;
                case 2:
                    std::cout << "Enter the book author you want to change:";
                    std::cin >> book_author;
                    book["author"] = book_author;

                break;
                case 3:
                    std::cout << "Enter the book description you want to change:";
                    std::cin >> book_des;
                    book["des"] = book_des;

                break;
                default:std::cout << "The operating erro! Invalid serial number" << std::endl;break;
            }
            break;

        case 4:
            book["operat_type"] = 4;
            cout << "Please input book id:";
            cin >> sTemp;
            book_id = judge_input(sTemp);

            if (book_id == -1) {
                return "q";
            }

            book["id"] = book_id;
            break;

        case 5:
            std::cout << "The operating erro!" << std::endl;
            break;

        default:std::cout << "The operating erro!" << std::endl;return "error";break;
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

    header_element.push_back("book_id");
    header_element.push_back("book_name");
    header_element.push_back("book_author");
    header_element.push_back("book_des");
    Json::Reader reader;
    Json::Value data;
    reader.parse(Str, data, false);
    
    string book_id = std::to_string(data["id"].asInt());
    std::string book_name = data["name"].asString();
    string book_author = data["author"].asString();
    string book_des = data["des"].asString();
    if(book_name.empty() || book_author.empty() || book_des.empty()){
        cout << Str << endl;
        return ;
    }

        
    Draw_line(header_element);

    for (int i = 0; i < header_element.size() ; i++) {
        cout << "| " << setw(header_element[i].size()) << setiosflags(ios::left) << setfill(' ') << header_element[i] << ' ';
    }

    cout << '|' << endl;

    Draw_line(header_element);

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

int judge_input(string s) {

    if (s == "q" || s == "Q") {
        return -1;
    }

    int num;

    while (!all_is_num(s)) {
        if (s == "q" || s == "Q") {
            return -1;
        } else {
            cout << "The input format is illegal. Please enter an integer" << endl;
            cout << "请重新输入：";
            std::cin >> s;
        }
    }
    num = atoi(s.c_str());
    return num;
}

int judge_Result(string &s){
    if(s.empty()) return -1; //error
    if(s[0] == 's' || s[1] == 's'){
        s = s.substr(7);
        return 0;
    }
    std::cout<<s<<std::endl;
    return 1;//failed
}

bool all_is_num(string str)
{
    for (int i = 0; i < str.size(); i++) {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}