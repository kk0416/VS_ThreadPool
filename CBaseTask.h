#pragma once
class CBaseTask
{
public:
    CBaseTask();
    CBaseTask(char* taskname);
    ~CBaseTask();
    void setData(char* data);  
    virtual int Run() = 0; //纯虚函数run：业务具体处理逻辑，一个纯虚函数由子类继承做各类业务

protected:
    char *taskname;//业务名字
    char *data;//业务具体数据

};

