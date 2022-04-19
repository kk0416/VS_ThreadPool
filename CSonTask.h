#pragma once
#include "CBaseTask.h"
#include <iostream>
#include <unistd.h>
using namespace std;
class CSonTask :
    public CBaseTask
{
public:
    CSonTask();
    CSonTask(char* taskname);
   ~CSonTask();
   int Run();

};

