/**
 * @file ServerStream.h
 * @author zhangshixin (zhangshixin@scutech.com)
 * @brief 定义一个客户端与服务器端的通道，当客户端与服务器发生连接则实例化该通道用于传输控制命令
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SERVERSTREAM_H_
#define SERVERSTREAM_H_
#pragma once

#include "MsgData.cpp"
#include "TaskThread.h"
#include "FtpMsg.h"
#include "User.h"
#include "FilesCtr.cpp"

#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Task.h>
#include <ace/Reactor.h>
#include <ace/OS.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Acceptor.h>

#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

#define COMMAND_SIZE 80


class ServerStream : public ACE_Event_Handler
{
private:
    /* data */
    ACE_SOCK_STREAM m_Svr_stream;
    //ACE_SOCK_STREAM *m_Data_stream;
    TaskThread *m_MsgQueue;

public:
    ServerStream(TaskThread *pMsgQueue);
    ~ServerStream();
    virtual int handle_input(ACE_HANDLE fd) override;
    void close();
    virtual ACE_HANDLE get_handle(void) const override;
    ACE_SOCK_STREAM &get_stream();
};


#endif