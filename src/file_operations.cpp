/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
//
// Created by flusgan on 2019/8/21.
//
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <string>


using namespace std;
void writefile(fstream &in,string FilePath,vector<string> str)
{
    in.open(FilePath.c_str(),ios::out|ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建   ios::app
    for(string s:str)
        in << s;
    in.close();//关闭文件
}

void readfile(fstream &in, string FilePath ,vector<string> data) {
    in.open(FilePath, ios::in);//打开一个file
    if (!in.is_open()) {
        cout << "Can not find " << FilePath << endl;
    }
    string buff;
    int i = 0;//行数i
    while (getline(in, buff)) {
        data.push_back(buff);
    }//end while
    in.close();
    
}


// int main(){
//     string str[]={"24153415,1241514,12124","gsisgfuidsfs,afasdfasd,sfsdsdf","1241235,dasgsda,gsdgds"};
//     fstream in;
//     int i;
//     for(i=0;i<3;i++)
//         writefile(in,str[i]);
//     int data[10][10];
//     readfile(in, "test.txt", data);
//     return 0;
// }

