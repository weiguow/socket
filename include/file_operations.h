/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
int writefile(const char * FilePath,const vector<string> &str);

int readfile(const char * FilePath ,vector<string> &data);