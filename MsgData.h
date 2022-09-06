#ifndef MSGDATA_H_
#define MSGDATA_H_
#pragma once

//#include "User.h"

#include <ace/Reactor.h>

#define MAX_BUFF_SIZE 1024

class MsgData
{
private:
    ACE_HANDLE *IOHandle;
    int DataFlog;
    //User fromUser;

public:
    MsgData(/* args */);
    ~MsgData();
    void setDataFlog(int flog);
    int getDataFlog() const;
    void setIOHandle(ACE_HANDLE *handle);
    ACE_HANDLE *getIOHandle() const;
    char Data[MAX_BUFF_SIZE];
    
};
#endif