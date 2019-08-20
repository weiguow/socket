/*
 * @Author: Liangyuhang
 * @LastEditors: Liangyuhang
 */

#pragma once 
 
#include <deque>
#include <string>  
#include <pthread.h>  
#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif
  
using namespace std;  
  
/** 
 * 执行任务的类，设置任务数据并执行 
 */  
class CTask {
protected:
    string m_strTaskName;   //任务的名称
    int connfd;    //接收的地址
 
public:
    CTask() = default;
    CTask(string &taskName): m_strTaskName(taskName), connfd(0) {}
    virtual pair<bool,std::string> Run(pthread_t) = 0;
    void SetConnFd(int data);   //设置接收的套接字连接号。
    int GetConnFd();
    virtual ~CTask() {}
    
};
 
 
 
 
/** 
 * 线程池类的实现 
 */  
class CThreadPool  
{  
private:  
    static  deque<CTask*> m_deqTaskList;     /** 任务队列 */  
    static  bool shutdown;                    /** 线程退出标志 */           
    int     m_iThreadNum;                     /** 线程池中启动的线程数 */  
    pthread_t   *pthread_id;  
      
    static pthread_mutex_t m_pthreadMutex;    /** 线程同步锁 */  
    static pthread_cond_t m_pthreadCond;      /** 线程同步的条件变量 */  
	static __gnu_cxx::hash_map<int,deque<pair<bool,std::string>>> taskMap;
  
protected:  
    static void* ThreadFunc(void * threadData); /** 新线程的线程回调函数 */  
    static int MoveToIdle(pthread_t tid);       /** 线程执行结束后，把自己放入到空闲线程中 */  
    static int MoveToBusy(pthread_t tid);       /** 移入到忙碌线程中去 */  
      
    int Create();          /** 创建线程池中的线程 */  
  
public:  
    CThreadPool(int threadNum = 10);  
	~CThreadPool();
    int AddTask(CTask *task);      /** 把任务添加到任务队列中 */  
    int StopAll();                 /** 使线程池中的线程退出 */  
    int getTaskSize();             /** 获取当前任务队列中的任务数 */
    deque<pair<bool,std::string>> getResuleQueue(int sockid){
		return taskMap[sockid];
	}  
};  

// 代理类,只暴露给别人用的
class CThreadPoolProxy: public CThreadPool
{
public:
	static CThreadPoolProxy* instance()
	{
		if(NULL == m_pInstance)
		{
			m_pInstance = new CThreadPoolProxy;
			return m_pInstance;
		}
		return m_pInstance;
	}
	int AddTask(CTask *task)
	{
		return m_pthreadpool->AddTask(task);
	}
	
private:
	CThreadPoolProxy()
	{
		m_pthreadpool = new CThreadPool(5);
	}
	~CThreadPoolProxy()
	{
		delete m_pthreadpool;
	}
	
private:
	static CThreadPoolProxy* m_pInstance;
	CThreadPool* m_pthreadpool;
};

 