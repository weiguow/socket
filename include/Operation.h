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
    int Run();
};

