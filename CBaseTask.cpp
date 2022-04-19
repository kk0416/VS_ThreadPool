#include "CBaseTask.h"
#include <string.h>

CBaseTask::CBaseTask()
{
    this->taskname = NULL;
    this->data = NULL;
}

CBaseTask::CBaseTask(char* taskname)
{
    strcpy(this->taskname,taskname);
    this->data = NULL;
}

CBaseTask::~CBaseTask()
{

}

void CBaseTask::setData(char* data)
{
    this->data = data;
}


