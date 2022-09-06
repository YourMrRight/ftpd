/**
 * @file Server_Stream.h
 * @author zhangshixin (zhangshixin@scutech.com)
 * @brief 定义一个客户端与服务器端的通道，当客户端与服务器发生连接则实例化该通道用于传输控制命令
 * @version 0.1
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __SERVER_STREAM_H__
#define __SERVER_STREAM_H__



#include "Msg_Data.h"
#include "Task_Thread.h"
#include "Ftp_Msg.h"
#include "User.h"

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


class Server_Stream : public ACE_Event_Handler
{
private:
    ACE_SOCK_STREAM m_svr_stream_;
    Task_Thread *m_msg_Queue_;

public:
    Server_Stream(Task_Thread *p_msg_queue);
    ~Server_Stream();
    virtual int handle_input(ACE_HANDLE fd) override;
    void close();
    virtual ACE_HANDLE get_handle(void) const override;
    ACE_SOCK_STREAM &get_stream();
};



#endif // __SERVER_STREAM_H__