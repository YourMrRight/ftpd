#include "Rmd_Command.h"
#include "User.h"
#include "Ftp_Msg.h"
#include "Util.h"

extern unordered_map<int, User*>handler_user;

void Rmd_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    
    User *current_user = handler_user[stream.get_handle()];
    string del_dir = cmd.substr(4,cmd.size());
    trim(del_dir);
    if (del_dir[0] != '/' && del_dir.size() > 1) {
        del_dir = current_user->get_current_dir() + del_dir;
    }
    if (rm_dir(del_dir.c_str())==-1) {
        stream.send(RMD_ERRROE, ACE_OS::strlen(RMD_ERRROE), 0);
        return;
    }
    stream.send(RMD_CMD,  ACE_OS::strlen(RMD_CMD), 0);
}
