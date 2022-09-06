#ifndef __SERVER_H__
#define __SERVER_H__

#include "Server_Stream.h"

#include <ace/Log_Msg.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Reactor.h>

#include <map>
#include <list>

class Server: public ACE_Event_Handler
{
private:
    ACE_INET_Addr m_Svr_addr;
    ACE_SOCK_Acceptor m_Svr_accept;
    list<Server_Stream *> stream_pool;
public:
    Server(int port,char* ip);
    virtual ~Server();
    bool open();
    void close();
    virtual ACE_HANDLE get_handle(void)const override;
    virtual int handle_input(ACE_HANDLE handle)override;
    void init();
};


#endif // __SERVER_H__