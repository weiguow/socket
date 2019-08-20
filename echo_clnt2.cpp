/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include "include/json/json.h"

#define BUF_SIZE 1024
#define MY_PORT 8150

using namespace std;

void error_handling(const char *message, const int sock_port);

void read_handling(const int sock);

void write_handling(const int sock);

std::string user_operator();

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in addr;
    pid_t pid;
    const char *address = (argc != 3 ? "127.0.0.1" : argv[1]);
    int nPort = (argc != 3 ? MY_PORT : atoi(argv[2]));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error", sock);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons(nPort);

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1)
        error_handling("connect() error", sock);

    pid = fork();
    if (pid == 0) {
        write_handling(sock);
    } else {
        read_handling(sock);
    }
    close(sock);
    return 0;
}

//写进程
void write_handling(int sock) {
    char buf[BUF_SIZE];
    while (1) {
        memset(buf, 0, BUF_SIZE);
        //fgets(buf, BUF_SIZE, stdin);

        cout << user_operator() << endl;

        write(sock, user_operator().c_str(), strlen(user_operator().c_str()));
        if (!strcmp(buf, "q\n") ||
            !strcmp(buf, "Q\n")) {
            shutdown(sock, SHUT_WR);
            return;
        }
    }
}

//读进程
void read_handling(const int sock) {
    int str_len;
    char buf[BUF_SIZE];
    while (1) {
        str_len = read(sock, buf, BUF_SIZE - 1);
        if (str_len <= 0)
            return;
        buf[str_len] = 0;
        printf("the message from server:%s", buf);
    }
}


void error_handling(const char *message, const int port) {
    fputs(message, stderr);
    fputc('\n', stderr);
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
    string book_name;  //书名
    string book_author;    //作者
    string book_des;   //简介

    cout << "===请选择操作编号====\n"
         << "1.新增图书\n"
         << "2.删除图书\n"
         << "3.改动图书\n"
         << "4.查询图书\n"
         << "输入编号：" << endl;
    cin >> operat_type;

    Json::Value book;
    string sbook;

        switch (operat_type) {
            case 1 : {
                book["operat_type"] = 1;
                cout << "请输入图书编号: ";
                cin >> book_id;
                book["id"] = book_id;
                cout << "请输入你想添加的书名： ";
                cin >> book_name;
                book["name"] = book_name;
                cout << "请输入该书作者： ";
                cin >> book_author;
                book["author"] = book_author;
                cout << "请输入该书简介： ";
                cin >> book_des;
                book["des"] = book_des;
                sbook = book.toStyledString();
                book.clear();
                break;
            }

                case 2 : {
                    book["operat_type"] = 2;
                    cout << "请输入图书编号: ";
                    cin >> book_id;
                    book["id"] = book_id;
                    sbook = book.toStyledString();
                    book.clear();
                    break;
                }

                case 3:  //改动图书
                    int change_num;
                    book["operat_type"] = 3;
                    cout << "请输入图书编号: ";
                    cin >> book_id;
                    book["id"] = book_id;
                    cout << "===请选择需要修改信息的项目====\n"
                         << "1.图书编号\n"
                         << "2.图书名称\n"
                         << "3.图书作者\n"
                         << "4.图书简介\n"
                         << "输入编号：" << endl;

                    cin >> change_num;

                    switch (change_num)
                    {
                            case 1:
                                cout << "请输入图书编号: ";
                                cin >> book_id;
                                book["id"] = book_id;
                            break;
                            case 2:
                                cout << "请输入你想修改的书名： ";
                                cin >> book_name;
                                book["name"] = book_name;
                            break;
                            case 3:
                                cout << "请输入修改后的作者名： ";
                                cin >> book_author;
                                book["author"] = book_author;
                            break;
                            case 4:
                                cout << "请输入新简介： ";
                                cin >> book_des;
                                book["des"] = book_des;
                            break;
                        default:cout << "The operating erro!" << endl;
                    }
                    sbook = book.toStyledString();
                    book.clear();
                    break;

                case 4:
                    book["operat_type"] = 4;
                    cout << "请输入图书编号: ";
                    cin >> book_id;
                    book["id"] = book_id;
                    sbook = book.toStyledString();
                    book.clear();
                    break;
            default:cout << "The operating erro!" << endl;
            }
    return sbook;
}
