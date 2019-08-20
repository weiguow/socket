/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#pragma once
#include "Thread.h"
#include <string>
enum Otype{
    ADD,
    DELETE,
    MODIFY,
    SEARCH
};
class Operation:public CTask
{
private:
    /* data */
    Otype opt;
    std::string sDoing;
public:
    Operation(/* args */);
    ~Operation();
    pair<bool,std::string> Run(pthread_t);
    inline int setOpt(Otype o){
        opt = o;
        return 0;
    };
    inline int setDoing(std::string sTemp){
        sDoing = sTemp;
        return 0;
    };
};

