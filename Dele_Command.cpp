#include "Dele_Command.h"
#include "User.h"
#include "Util.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*> handler_user;

void Dele_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    
    User *current_user = handler_user[stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if (dir[0] != '/') { 
        dir = current_user->get_current_dir() + dir;
    }
    FILE *fp;
    fp = ACE_OS::fopen(dir.c_str(), "r");
    if (fp == nullptr) {
        stream.send(DELETE_FAILE, ACE_OS::strlen(DELETE_FAILE), 0);
        return;
    }
    if (remove(dir.c_str())!=0) {
        stream.send(DELETE_FAILE, ACE_OS::strlen(DELETE_FAILE), 0);
        return;
    }
    stream.send(DELETE_SUCCESS, ACE_OS::strlen(DELETE_SUCCESS), 0);
}
