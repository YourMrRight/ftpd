/**
 * @file Server.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Server.h"
#include "ServerStream.cpp"
#include "User.cpp"
#include "FtpMsg.h"

#include <ace/OS.h>
#include <ace/Task.h>

#define COMMAND_SIZE 80
#define FILE_DATA_BLOCK 1024
#define REC_BLOCK 2048
#define THREAD_NUM 10
//class user;
typedef ACE_Singleton<TaskThread,ACE_Thread_Mutex> TaskThreadPool;
list<ServerStream*>g_StreamPool;

Server::Server(int port,char* ip): m_Svr_addr(port,ip)
{
    if (!open())    //open listen port
    {
        ACE_DEBUG((LM_INFO, ACE_TEXT("open failed!\n")));
    } else
    {
        ACE_DEBUG((LM_INFO, ACE_TEXT("open success!\n")));
        TaskThreadPool::instance()->activate(THR_NEW_LWP | THR_JOINABLE |THR_INHERIT_SCHED , THREAD_NUM);//创建10个线程处理业务
    }
}

Server::~Server()
{
    close();
    std::list<ServerStream*>::iterator it;
    for (it = g_StreamPool.begin();it != g_StreamPool.end();++it)
    {
        if (NULL != (*it))
        {
            (*it)->close();
            delete (*it);
        }
    }
}

bool Server::open()
{
    if (-1 == m_Svr_accept.open(m_Svr_addr,1))
    {
        ACE_DEBUG((LM_ERROR,ACE_TEXT("failed to accept\n")));
        m_Svr_accept.close();
        return false;
    }
    return true;
}

void Server::close()
{
    ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::ACCEPT_MASK);
    m_Svr_accept.close();
}

ACE_HANDLE Server::get_handle(void) const 
{
    return m_Svr_accept.get_handle();   
}

int Server::handle_input(ACE_HANDLE handle)
{
    ServerStream *stream = new ServerStream(TaskThreadPool::instance());    //产生新通道
    if (NULL != stream)
    {
        g_StreamPool.push_back(stream);//暂时存储全局变量用于内存管理,优化可增加一个连接管理类管理连接通道
    }
    if (m_Svr_accept.accept(stream->get_stream()) == -1)  //绑定通道
    {  
        printf("accept client fail\n");  
        return -1;  
    }
    ACE_Reactor::instance()->register_handler(stream,ACE_Event_Handler::READ_MASK);  //通道注册到ACE_Reactor
    stream->get_stream().send(ESTABLISH,ACE_OS::strlen(ESTABLISH));
    ACE_DEBUG((LM_INFO,"User connect success!,ClientPool num = %d\n",g_StreamPool.size()));
    return 0; 
}