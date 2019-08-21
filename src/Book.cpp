/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include "Book.h"
#include <iostream>

Map* Map::m_pInstance = NULL;
__gnu_cxx::hash_map<int,Book> Map::myMap;


Book::Book(/* args */)
{
}

Book::~Book()
{
    
}
//json format : {"age":100,"name":"hello world"}
std::string Book::to_string()const{
    std::string s = "";
    s += "{\"id\":";
    s += std::to_string(nBookID);
    s += ",\"name\":\"" + sBookName +"\"";
    s += ",\"author\":\"" + sAuthor + "\"";
    s += ",\"des\":\"" + sBookDes + "\"}";
    return s;
}

int Map::insertMap(Book b){
    myMap.insert(make_pair(b.getBookId(),b));
    return 0;
}

int Map::DeteleMap(const int bId){
    myMap.erase(myMap.find(bId));
    return 0;
}

int Map::searchMap(const int bId,Book &b)const{
    auto it = myMap.find(bId);
    if(it!=myMap.end()){ //find it
        b = it->second;
        return 0;
    }
    else 
        return -1;
}
    
int Map::modifyMap(const int bId,const int type,const std::string s)
{
    auto it = myMap.find(bId);
    if(it == myMap.end())
        return -1;
    switch (type)
    {
    case 1:
        it->second.setsBookName(s);
        break;
    case 2:
        it->second.setsAuthor(s);
        break;
    case 3:
        it->second.setsBookDes(s);
        break;
    default:
        break;
    }
    return 0;
}