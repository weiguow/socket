/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include <iostream>
#include "Operation.h"
#include "Book.h"



pair<bool,std::string> Operation::Run(pthread_t tid){
    //TODO the opeation about the object
    Book b;
    bool returnflag = false;
    std::string retMessage = "";
    int ntempID;
    switch(opt){
        case ADD:
            b.setBookId(jData["id"].asInt());
            b.setsBookName(jData["name"].asString());
            b.setsAuthor(jData["author"].asString());
            b.setsBookDes(jData["des"].asString());
            if(Map::instance()->insertMap(b) == -1)
            {
                returnflag = false;
                retMessage = "id is 重复";
            }
            else{
                returnflag = true;
                retMessage = "success";
            }
        break;
        case DELETE:
            if(Map::instance()->DeteleMap(jData["id"].asInt()) == 0){
                returnflag = true;
                retMessage = "success";
            }
            else{
                returnflag = false;
                retMessage = "no this id";
            }
        break;
        case MODIFY:
            
        break;
        case SEARCH:
            if(Map::instance()->searchMap(jData["id"].asInt(),b) == 0){
                returnflag = true;
                retMessage = b.to_string();
            }
            else{
                returnflag = false;
                retMessage = "Not found";
            }
        break;
        default:
            retMessage = "error operation type!";
        break;
    }
    //retMessage += "\n";
    // std::cout<<tid<<opt<<" "<<sDoing<<std::endl;
    pair<bool,std::string> r (returnflag,retMessage);
    return r;
}

Operation::Operation(/* args */)
{
}

Operation::~Operation()
{
}

int Operation::add(Book b){
    
}