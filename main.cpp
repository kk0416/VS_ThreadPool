#include "CThreadPool.h"
#include "CSonTask.h"
#include <iostream>

using namespace std;

int main() 
{
	CThreadPool* pool = new CThreadPool(10);

	for (int i = 0; i < 30; i++)
	{
		CSonTask* stask = new CSonTask();
		char buf[50] = { "test ThreadPool。。。。。" };
		stask->setData(buf);
		pool->addTask(stask);
		cout << "task" << i + 1 << endl;

	}

	while (1)
	{

	}
	cout << "操作完成" <<endl;
	return 0;
}