#pragma once
#include "CBaseTask.h"
#include <iostream>
#include <pthread.h>
#include <queue>
#include <list>
#include <algorithm>//list的find函数需要
using namespace std; 
#define THREAD_MAX_NUM 50//线程数量的最大数量值
class CThreadPool
{
public:
	CThreadPool(int threadMinNum);
	~CThreadPool();
	int stopAll();//停止线程池
	int create();//创建线程
	int addTask(CBaseTask* task);//添加任务到任务队列
protected:
	static void* pthread_function(void* p);//线程执行函数
	static int MoveToIdle(pthread_t tid);
	static int MoveToBusy(pthread_t tid);

private:
	int threadMinNum;
	int threadMaxNum;
	queue<CBaseTask*>taskQueue;//任务队列
	static list<pthread_t>idleThread;//空闲队列
	static list<pthread_t>busyThread;//忙碌队列
	static pthread_mutex_t idleMutex;//互斥量
	static pthread_mutex_t busyMutex;
	static pthread_mutex_t pthreadMutex;//线程、任务互斥量
	static pthread_cond_t pthreadCond;//线程条件变量：线程的等待与唤醒


};

