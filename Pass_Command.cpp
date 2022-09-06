#include "Pass_Command.h"
#include "User.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

#include <unordered_map>

using namespace std;

extern unordered_map<int, User*> handler_user;

void Pass_Command::exec(const ACE_SOCK_STREAM &stream, const string &cmd)
{
    if (handler_user.find(stream.get_handle()) == handler_user.end()) {
        return;
    }

    string file_pwd = handler_user[stream.get_handle()]->get_pwd(handler_user[stream.get_handle()]->get_user_name());

    string password = cmd.substr(5,cmd.size());

    trim(password);

    if (file_pwd == password){
        handler_user[stream.get_handle()]->set_status(1);
        handler_user[stream.get_handle()]->init();
        stream.send(LOGIN_SUCCESS, ACE_OS::strlen(LOGIN_SUCCESS));
    } else {
        if (stream.get_handle() == -1) {
            cout<<"通道已经关闭"<<endl;
        }
        ACE_DEBUG((LM_DEBUG, "user %s login failed! \n", handler_user[stream.get_handle()]->get_user_name()));
        if (handler_user.find(stream.get_handle()) != handler_user.end()) {
            User *del = handler_user[stream.get_handle()];
            handler_user.erase(stream.get_handle());
            delete del;
        }
        stream.send(USR_LOGI_FAILED, ACE_OS::strlen(USR_LOGI_FAILED));
    }
}
