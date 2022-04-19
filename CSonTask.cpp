#include "CSonTask.h"

CSonTask::CSonTask():CBaseTask()
{
}

CSonTask::CSonTask(char* taskname) : CBaseTask(taskname)
{
}

CSonTask::~CSonTask()
{
}

int CSonTask::Run()
{
	//先不做具体业务 打印一下
	cout << this->data << endl;
	//模拟做业务的时间
	sleep(3);
	return 0;
}
