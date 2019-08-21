/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */
#pragma once
#include <string>
#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif
using namespace std;
class Book
{
private:
    /* data */
    int nBookID;
    string sBookName;
    string sAuthor;
    string sBookDes;
public:
    Book(/* args */);
    ~Book();
};


class Map
{
public:
	static Map* instance()
	{
		if(NULL == m_pInstance)
		{
			m_pInstance = new Map;
			return m_pInstance;
		}
		return m_pInstance;
	}
	
    __gnu_cxx::hash_map<int,Book> getMap(){
        return myMap;
    }
private:
	Map()
	{
	
	}
	~Map()
	{
		
	}

private:
	static Map* m_pInstance;
    static __gnu_cxx::hash_map<int,Book> myMap;
	//CThreadPool* m_pthreadpool;
};
