#ifndef SERVER_H_
#define SERVER_H_
#pragma once

#include "ServerStream.h"

#include <ace/Log_Msg.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Reactor.h>

#include <map>
#include <list>

using namespace std;

class Server: public ACE_Event_Handler
{
private:
    /* data */
    ACE_INET_Addr m_Svr_addr;
    ACE_SOCK_Acceptor m_Svr_accept;
public:
    Server(int port,char* ip);
    virtual ~Server();
    bool open();
    void close();
    virtual ACE_HANDLE get_handle(void)const override;
    virtual int handle_input(ACE_HANDLE handle)override;
    //virtual int handle_signal(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
};


#endif