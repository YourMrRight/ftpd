#include "Task_Thread.h"
#include "Util.h"
#include "Command.h"
#include "Invoker.h"
#include "Command_Factory.h"
#include "User.h"
#include "Msg_Data.h"
#include "Server_Stream.h"

#include <unordered_map>
#include <string>
#include <list>
#include <cctype>
#include <algorithm>

using namespace std;

unordered_map<int, User*> handler_user;
extern list<Server_Stream*> g_stream_pool;

int Task_Thread::process_cmd(ACE_SOCK_Stream stream, string cmd)
{
    //ACE_DEBUG((LM_DEBUG,"current command:%s",cmd));
    cout<<"-------"<<cmd<<"-----------"<<endl;
    Invoker *invoker = new Invoker();
    string cmd_name = cmd.substr(0,cmd.find_first_of(" "));
    toUpper(cmd_name);
    Command *cmd_obj = Command_Factory::getInstance()->get_cmd_obj(cmd_name);

    if(cmd_obj == nullptr){
        //throw std::runtime_error("can't not process this command");
        ACE_DEBUG((LM_DEBUG,"No such command:%s\n",cmd_name));
        return 0;
    }
    invoker->set_command(cmd_obj);
    try
    {
        invoker->action(stream, cmd);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        delete invoker;
    }
    delete invoker;
    
    return 0;
}


int Task_Thread::svc(void)
{
    ACE_Message_Block *Msg;
    while (true)
    {
        getq(Msg);
        ACE_Data_Block *Data_Block = Msg->data_block();
        Msg_Data *pData = reinterpret_cast<Msg_Data *>(Data_Block->base());
        if (0 == pData->get_data_flag()) {
            std::list<Server_Stream *>::iterator it;
            for (it = g_stream_pool.begin(); it != g_stream_pool.end(); ++it) {
                if (get_handle() == (*it)->get_handle()) {
                    g_stream_pool.erase(it);
                    delete *it;
                    break;
                }
            }
            return 0;
        }
        char strBuffer[MAX_BUFF_SIZE];
        ACE_OS::memset(strBuffer, 0, sizeof(strBuffer));
        ACE_OS::memcpy(strBuffer, pData->data, sizeof(strBuffer));
        if (this->process_cmd(*(pData->get_io_handle()), string(pData->data)) == -1) {
            ACE_DEBUG((LM_ERROR, "error!\n"));
            ACE_OS::exit(-1);
        }

        Msg->release();
    }
    return 0;
}
