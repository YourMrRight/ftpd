#include "Rnto_Command.h"
#include "User.h"
#include "Ftp_Msg.h"
#include "Util.h"

extern unordered_map<int, User*>handler_user;

void Rnto_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    
    User *current_user = handler_user[stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if(dir[0] !='/' && dir.size() > 1){
        dir = current_user->get_current_dir() + dir;
    }
    if (rename(current_user->get_rename_from().c_str(),dir.c_str()) < 0) {
        stream.send(RNTO_FAILED,ACE_OS::strlen(RNTO_FAILED), 0);
        return;
    }
    stream.send(RENAME_SUCCESS, ACE_OS::strlen(RENAME_SUCCESS), 0);

}
