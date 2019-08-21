/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include <iostream>
#include "Operation.h"



pair<bool,std::string> Operation::Run(pthread_t tid){
    //TODO the opeation about the object
    switch(opt){
        case ADD:
            
        break;
        case DELETE:
        break;
        case MODIFY:
        break;
        case SEARCH:
        break;
        default:
        break;
    }
    std::cout<<tid<<opt<<" "<<sDoing<<std::endl;
    pair<bool,std::string> r (true," test ");
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