/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include <iostream>
#include "Operation.h"
pair<bool,std::string> Operation::Run(pthread_t tid){
    //TODO the opeation about the object
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
