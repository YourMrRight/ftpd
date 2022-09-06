#include "Pwd_Command.h"
#include "User.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*> handler_user;

void Pwd_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    User *user = handler_user[stream.get_handle()];
    string currentDir;
    currentDir.append(PWD_CMD).append(user->get_current_dir()).append(" is the current directory.\n");
    stream.send(currentDir.c_str(), currentDir.size(), 0);
}
