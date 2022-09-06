#include "MsgData.h"


MsgData::MsgData(/* args */)
{
    IOHandle = nullptr;
    DataFlog = -1;
    ACE_OS::memset(Data, 0, sizeof(Data));
}

MsgData::~MsgData()
{
}

void MsgData::setDataFlog(int flog)
{
    this->DataFlog = flog;
}

int MsgData::getDataFlog() const
{
    return this->DataFlog;
}

void MsgData::setIOHandle(ACE_HANDLE *handle)
{
    this->IOHandle = handle;
}   

ACE_HANDLE *MsgData::getIOHandle() const
{
    return IOHandle;
}
