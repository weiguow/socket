/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#include "Book.h"
#include <iostream>
#include "json/json.h"

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


Map::Map():count(0){
    
}

Map::~Map(){
    
}


int Map::insertMap(Book b){
    auto it = myMap.find(b.getBookId());
    if(it == myMap.end()){
        myMap.insert(make_pair(b.getBookId(),b));
        count++;
        return 0;
    }
    else
        return -1;
}

int Map::DeteleMap(const int bId){
    auto it = myMap.find(bId);
    if(it!=myMap.end()){ //find it
        myMap.erase(it);
        return 0;
    }
    else 
        return -1;
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
    
int Map::modifyMap(const int bId,MTYPE type,const std::string s)
{
    auto it = myMap.find(bId);
    if(it == myMap.end())
        return -1;
    switch (type)
    {
    case NAME:
        it->second.setsBookName(s);
        break;
    case AUTHOR:
        it->second.setsAuthor(s);
        break;
    case DES:
        it->second.setsBookDes(s);
        break;
    default:
        break;
    }
    return 0;
}