/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#pragma once
#include "Thread.h"
#include <string>
#include "json/json.h"
#include "Book.h"
#include <map>

enum OTYPE{
    ADD,
    DELETE,
    MODIFY,
    SEARCH
};

class Operation:public CTask
{
private:
    /* data */
    OTYPE opt;
    std::string sDoing;
    Json::Value jData;
    MTYPE modify_type;
    std::map<MTYPE,std::string> modifyMap;
public:
    Operation(/* args */);
    ~Operation();
    pair<bool,std::string> Run(pthread_t);
    inline int setOpt(OTYPE o){
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
    inline int setModifyType(MTYPE m){
        modify_type = m;
        return 0;
    }
private:
    int add(Book b);
    int delete_book(int bId);
    int modify(int bId);
    Book search(int bId);
};

