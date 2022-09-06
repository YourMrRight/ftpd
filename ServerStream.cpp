#include "ServerStream.h"
#include "TaskThread.cpp"


using namespace std;

extern unordered_map<int, User *> handlerToUser;

ServerStream::ServerStream(TaskThread *pMsgQueue)
{
    this->m_MsgQueue = pMsgQueue;
}

ServerStream::~ServerStream()
{
    this->close();
}

int ServerStream::handle_input(ACE_HANDLE fd)
{
    MsgData Message;
    char strBuffer[MAX_BUFF_SIZE];
    Message.setDataFlog(m_Svr_stream.recv(strBuffer, MAX_BUFF_SIZE));
    if (Message.getDataFlog() == -1)
    {
        ACE_DEBUG((LM_INFO, ACE_TEXT("recive data error!\n")));
        return -1;
    }
    else if (Message.getDataFlog() == 0)
    {
        if (handlerToUser.find(get_handle()) != handlerToUser.end())
        {
            User *del = handlerToUser[get_handle()];
            handlerToUser.erase(get_handle());
            delete del;
        }
        close();
        ACE_DEBUG((LM_INFO, ACE_TEXT("client closed!\n")));
    }
    ACE_Data_Block *Data_Block = new ACE_Data_Block; //线程做释放
    ACE_HANDLE cli_io = get_handle();
    Message.setIOHandle(&cli_io);
    ACE_OS::memcpy(Message.Data, strBuffer, sizeof(strBuffer)); //传的data可带length信息来适配消息大小

    char *p = reinterpret_cast<char *>(&Message);
    Data_Block->base(p, sizeof(Message));
    ACE_Message_Block *msg = new ACE_Message_Block(Data_Block);
    m_MsgQueue->putq(msg); // put
    return 0;
}

void ServerStream::close()
{
    m_Svr_stream.close();
    ACE_Reactor::instance()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
}

ACE_HANDLE ServerStream::get_handle(void) const
{
    return m_Svr_stream.get_handle();
}

ACE_SOCK_STREAM &ServerStream::get_stream()
{
    return m_Svr_stream;
}
