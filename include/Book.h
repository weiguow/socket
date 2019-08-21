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
    inline int getBookId() const{
        return nBookID;
    }
    inline void setBookId(const int bID){
        nBookID = bID;
    }
    inline void setsBookName(const std::string s){
        sBookName = s;
    }
    inline void setsAuthor(const std::string s){
        sAuthor = s;
    }
    inline void setsBookDes(const std::string s){
        sBookDes = s;
    }
    std::string to_string()const;
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
    
    int insertMap(Book b);
    int DeteleMap(const int bId);
    int searchMap(const int bId,Book &) const;
    int modifyMap(const int bId,const int type,const std::string s);
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
