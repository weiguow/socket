/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#pragma once
#include "Thread.h"
#include <string>
#include "json/json.h"
#include "Book.h"

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
    Json::Value jData;
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
    inline int setData(Json::Value Data){
        jData = Data;
        return 0;
    }
private:
    int add(Book b);
    int delete_book(int bId);
    int modify(int bId);
    Book search(int bId);
};

