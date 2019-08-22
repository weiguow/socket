/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
//
// Created by flusgan on 2019/8/21.
//
#include "file_operations.h"


int writefile(const char * FilePath,const vector<string> &str)
{
    if(str.empty()) return 0;
    fstream in;
    in.open(FilePath,ios::out|ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建   ios::app
    for(string s:str)
        in << s ;
    in.close();//关闭文件
    return 0;
}

int readfile(const char * FilePath,vector<string> &data) {
    fstream in;
    in.open(FilePath, ios::in);//打开一个file
    if (!in.is_open()) {
        cout << "Can not find " << FilePath << endl;
        return -1;
    }
    string buff;
    while (getline(in, buff)) {
        data.push_back(buff);
    }//end while
    in.close();
    return 0;
}



