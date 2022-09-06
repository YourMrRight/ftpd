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
#include "Server_Stream.h"
#include "User.h"
#include "Ftp_Msg.h"

#include <ace/OS.h>
#include <ace/Task.h>

#define COMMAND_SIZE 80
#define FILE_DATA_BLOCK 1024
#define REC_BLOCK 2048
#define THREAD_NUM 10

typedef ACE_Singleton<Task_Thread,ACE_Thread_Mutex> TaskThreadPool;

list<Server_Stream*>g_stream_pool;

Server::Server(int port,char* ip)
    : m_Svr_addr(port,ip)
{

}

Server::~Server()
{
    close();
    for (auto i : g_stream_pool) {
        i->close();
    }
}

bool Server::open()
{
    if (m_Svr_accept.open(m_Svr_addr, 1) == -1) {
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
    Server_Stream *stream = new Server_Stream(TaskThreadPool::instance());    //产生新通道
    if (m_Svr_accept.accept(stream->get_stream()) == -1) {  
        printf("accept client fail\n");  
        delete stream;
        return -1;  
    }
    if (!stream) {
        g_stream_pool.push_back(stream);//暂时存储全局变量用于内存管理,优化可增加一个连接管理类管理连接通道
        stream_pool.push_back(stream);
    }

    ACE_Reactor::instance()->register_handler(stream, ACE_Event_Handler::READ_MASK);  //通道注册到ACE_Reactor
    stream->get_stream().send(ESTABLISH,ACE_OS::strlen(ESTABLISH));
    ACE_DEBUG((LM_INFO, "User connect success! ClientPool num = %d\n", g_stream_pool.size()));
    return 0; 
}

void Server::init()
{
    if (!open()) {
        ACE_DEBUG((LM_INFO, "open failed!\n"));
    } else {
        ACE_DEBUG((LM_INFO, "open success!\n"));
        TaskThreadPool::instance()->activate(THR_NEW_LWP | THR_JOINABLE |THR_INHERIT_SCHED , THREAD_NUM);//创建10个线程处理业务
    }
}
