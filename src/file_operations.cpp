//
// Created by flusgan on 2019/8/21.
//
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>


using namespace std;
void writefile(fstream &in,string str){
in.open("test.txt",ios::out|ios::app); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建   ios::app
in<<str<<"\n";
in.close();//关闭文件
}

void readfile(fstream &in, string FilePath, int data[10][10]) {
    in.open(FilePath, ios::in);//打开一个file
    if (!in.is_open()) {
        cout << "Can not find " << FilePath << endl;
        system("pause");
    }
    string buff;
    int i = 0;//行数i
    while (getline(in, buff)) {
        vector<double> nums;
        // string->char *
        char *s_input = (char *)buff.c_str();
        const char * split = "\n";
        // 以‘,’为分隔符拆分字符串
        char *p = strtok(s_input, split);
     double a;
        while (p != NULL) {
            // char * -> int
            a = atof(p);
            cout << p << endl;
            nums.push_back(a);
            p = strtok(NULL, split);
        }//end while
        //for (int b = 0; b < nums.size(); b++) {
         //   data[i][b] = nums[b];
        //}//end for        i++;
    }//end while
    in.close();
    cout << "get  data" << endl;
}


int main(){
    string str[]={"24153415,1241514,12124","gsisgfuidsfs,afasdfasd,sfsdsdf","1241235,dasgsda,gsdgds"};
    fstream in;
    int i;
    for(i=0;i<3;i++)
        writefile(in,str[i]);
    int data[10][10];
    readfile(in, "test.txt", data);
    return 0;
}

