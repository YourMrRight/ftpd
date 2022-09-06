#include "Rnfr_Command.h"
#include "Util.h"
#include "User.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*>handler_user;

void Rnfr_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    
    User *current_user = handler_user[stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if (dir[0] !='/' && dir.size() > 1) {
        dir = current_user->get_current_dir() + dir;
    }
    FILE *fp;
    fp = ACE_OS::fopen(dir.c_str(), "r");
    if (fp == nullptr) {
        stream.send(RNFR_FAILED, ACE_OS::strlen(RNFR_FAILED), 0);
        return;
    }
    current_user->set_rename_from(dir);
    stream.send(RNFR_CMD,ACE_OS::strlen(RNFR_CMD),0);
}
