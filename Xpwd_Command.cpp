#include "Xpwd_Command.h"
#include "User.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

extern unordered_map<int, User*>handler_user;

void Xpwd_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    User *user = handler_user[stream.get_handle()];
    char *dir_root = new char[COMMAND_SIZE];
    std::string str("257");
    str.append(user->get_current_dir());
    str.append(" is current directory.\r\n");
    ACE_OS::strcpy(dir_root, str.c_str());
    if (stream.send(dir_root, ACE_OS::strlen(dir_root), 0)==-1) {
        return;
    }
    ACE_DEBUG((LM_DEBUG, "user %s display current directory\n", user->get_user_name()));

    delete[] dir_root;
}
