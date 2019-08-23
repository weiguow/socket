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
    switch(opt){
        case ADD:
            b.setBookId(Map::instance()->getMap().size());
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
                retMessage = "ADD success";
            }
        break;
        case DELETE:
            if(Map::instance()->DeteleMap(jData["id"].asInt()) == 0){
                returnflag = true;
                retMessage = "DELETE success";
            }
            else{
                returnflag = false;
                retMessage = "no this id";
            }
        break;
        case MODIFY:
            if(Map::instance()->modifyMap(jData["id"].asInt(),modify_type,jData[modifyMap[modify_type]].asString()) == -1){
                returnflag = false;
                retMessage = "modify error";
            }
            else{
                returnflag = true;
                retMessage = "MODIFY success";
            }
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
            returnflag = false;
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
    modifyMap.insert(pair<MTYPE,std::string>(AUTHOR,"author"));
    modifyMap.insert(pair<MTYPE,std::string>(NAME,"name"));
    modifyMap.insert(pair<MTYPE,std::string>(DES,"des"));
}

Operation::~Operation()
{
}

