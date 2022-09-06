#include "Server_Stream.h"
#include "Task_Thread.h"
#include "Command.h"

using namespace std;

extern unordered_map<int, User *> handler_user;

Server_Stream::Server_Stream(Task_Thread *p_msg_queue)
    :m_msg_Queue_(p_msg_queue)
{
}

Server_Stream::~Server_Stream()
{
    this->close();
}

/**
 * @brief 通道有读事件发生的时候调用该函数读取通道数据，并将数据和通道id封装成MsgData
 * 放入任务TaskThread消息队列中
 * 
 * @param fd 
 * @return int 
 */
int Server_Stream::handle_input(ACE_HANDLE fd)
{
    Msg_Data Message;
    char strBuffer[MAX_BUFF_SIZE];
    Message.set_data_flag(m_svr_stream_.recv(strBuffer, MAX_BUFF_SIZE));
    if (Message.get_data_flag() == -1) {
        ACE_ERROR_RETURN((LM_ERROR, "Failed to recive data!\n"), -1);
    } else if (Message.get_data_flag() == 0) {
        if (handler_user.find(get_handle()) != handler_user.end()) {
            User *del = handler_user[get_handle()];
            handler_user.erase(get_handle());
            delete del;
        }
        close();
        ACE_DEBUG((LM_INFO, ACE_TEXT("client closed!\n")));
    }
    ACE_Data_Block *Data_Block = new ACE_Data_Block; //线程做释放
    ACE_HANDLE cli_io = get_handle();
    Message.set_io_handle(&cli_io);
    ACE_OS::memcpy(Message.data, strBuffer, sizeof(strBuffer)); //传的data可带length信息来适配消息大小

    char *p = reinterpret_cast<char *>(&Message);
    Data_Block->base(p, sizeof(Message));
    ACE_Message_Block *msg = new ACE_Message_Block(Data_Block);
    m_msg_Queue_->putq(msg); // put
    return 0;
}

void Server_Stream::close()
{
    m_svr_stream_.close();
    ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
}

ACE_HANDLE Server_Stream::get_handle(void) const
{
    return m_svr_stream_.get_handle();
}

ACE_SOCK_STREAM &Server_Stream::get_stream()
{
    return m_svr_stream_;
}
