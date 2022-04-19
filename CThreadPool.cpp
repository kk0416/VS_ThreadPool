#include "CThreadPool.h"

//静态成员初始化
list<pthread_t>CThreadPool::idleThread;
list<pthread_t>CThreadPool::busyThread;

pthread_mutex_t CThreadPool::idleMutex=PTHREAD_MUTEX_INITIALIZER;//互斥量
pthread_mutex_t CThreadPool::busyMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t CThreadPool::pthreadMutex = PTHREAD_MUTEX_INITIALIZER;//线程、任务互斥量
pthread_cond_t CThreadPool::pthreadCond = PTHREAD_COND_INITIALIZER;//线程条件变量：线程的等待与唤醒

CThreadPool::CThreadPool(int threadMinNum)
{
    cout << "CThreadPool" << endl;
    this->threadMinNum = threadMinNum;
    this->threadMaxNum = THREAD_MAX_NUM;
    this->create();


}

    CThreadPool::~CThreadPool()
{

}

void* CThreadPool::pthread_function(void* p)
{
    pthread_t tid = pthread_self();//获取线程自己的tid
    cout << "1该线程tid=" << tid << "    run------" << endl;
    cout << "pthread_function" << endl;
    //参数赋值
    queue<CBaseTask*>* taskQueue = (queue<CBaseTask*>*)p;
    while (1)
    {
        cout << "pthread_function while lock " << endl;
        pthread_mutex_lock(&pthreadMutex);
        //线程等待，之前先锁
        cout << "pthread_function while pthread_cond_wait s " << endl;
        pthread_cond_wait(&pthreadCond, &pthreadMutex);
        //测试 是否等待
        cout << "22该线程tid=" << tid << "    run------";
        //移到忙碌队列
        MoveToBusy(tid);
        //取任务
        CBaseTask* task = taskQueue->front();
        taskQueue->pop();
        pthread_mutex_unlock(&pthreadMutex);

        //打印个队列剩下数
        cout << "now Taskqueue  num=  " << taskQueue->size() << endl;
        cout << "now idlelist  num=  " << idleThread.size() << endl;
        cout << "now busylist  num=  " << busyThread.size() << endl;

        cout << "----------start run-----------" << endl;

        task->Run();//执行任务
        cout << "----------run stop-----------" << endl;
        cout << "pthread tid   " << tid << "is ---idle--- MoveToIdle" << endl;

        //移到空闲
        MoveToIdle(tid);

        if (taskQueue->size()>0 && idleThread.size()>0)
        {
            cout << "该线程休息。让其他线程去做剩余的任务 或者 该线程做下个任务" << endl;
            //唤醒其他线程去做任务
            pthread_cond_signal(&pthreadCond);
            //MoveToBusy(tid);
        }
      /*  else if (taskQueue->size() > 0 && idleThread.size() == 0)
        {
            cout << "新创建线程" << endl;
        }
        else if (taskQueue->size() == 0 && idleThread.size()>0)
        {
            cout << "销毁线程" << endl;
        }*/
    }
    return nullptr;
}

int CThreadPool::MoveToIdle(pthread_t tid)
{
    cout << "MoveToIdle" << endl;
    list<pthread_t>::iterator iter;
    iter = find(busyThread.begin(), busyThread.end(), tid);
    if (iter != busyThread.end())//查找成功，开始移动到忙碌队列
    {
        //忙碌移除
        pthread_mutex_lock(&busyMutex);
        busyThread.erase(iter);
        pthread_mutex_unlock(&busyMutex);
        //空闲添加
        pthread_mutex_lock(&idleMutex);
        idleThread.push_back(tid);
        pthread_mutex_unlock(&idleMutex);
    }
    return 0;
}

int CThreadPool::MoveToBusy(pthread_t tid)
{
    cout << "MoveToBusy" << endl;
    list<pthread_t>::iterator iter;
    iter = find(idleThread.begin(),idleThread.end(),tid);
    if (iter!=idleThread.end())//查找成功，开始移动到忙碌队列
    {
        //空闲移除
        pthread_mutex_lock(&idleMutex);
        idleThread.erase(iter);
        pthread_mutex_unlock(&idleMutex);
        //忙碌添加
        pthread_mutex_lock(&busyMutex);
        busyThread.push_back(tid);
        pthread_mutex_unlock(&busyMutex);
    }
    return 0;
}

int CThreadPool::stopAll()
{
    return 0;
}

//线程创建，进空闲队列
int CThreadPool::create()
{
    cout << "create" << endl;

    for (int i = 0; i < this->threadMinNum; i++)
    {
        pthread_t tid = 0;
        pthread_create(&tid,NULL, pthread_function,&taskQueue);
        idleThread.push_back(tid);
        cout << "create " << i<<endl;

    }

    return 1;
}

int CThreadPool::addTask(CBaseTask* task)
{
    cout << "addTask" << endl;
    //添加任务
    this->taskQueue.push(task);
    //判断空闲队列是否有线程
    if (idleThread.size()>0)
    {
        //唤醒等待的线程，条件变量---此时线程执行函数开始执行
        pthread_cond_signal(&pthreadCond);
        cout << "唤醒：pthread_cond_signal" << endl;

    }
    return 0;
}
