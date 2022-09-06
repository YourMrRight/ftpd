#include "Cdup_Command.h"
#include "User.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

extern unordered_map<int, User*>handler_user;

void Cdup_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    
    User *current_user = handler_user[stream.get_handle()]; 
    if (current_user->get_current_dir() == ROOT_DIR) {
        stream.send(CWD_SUCCESS, ACE_OS::strlen(CWD_SUCCESS));
    } else {
        string currentdir = current_user->get_current_dir();
        int length = currentdir.size()-2;
        while (current_user->get_current_dir()[length] != '/' && length >= 0) {
            length--;
        }
        current_user->set_current_dir(currentdir.substr(0,length).append("/"));
        stream.send(CWD_SUCCESS, ACE_OS::strlen(CWD_SUCCESS));
    }
    ACE_DEBUG((LM_DEBUG, "user %s modified directory!\n", current_user->get_user_name())); 
}
