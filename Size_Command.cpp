#include "Size_Command.h"
#include "User.h"
#include "Util.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*>handler_user;

void Size_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
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
        stream.send(DIR_NOT_FIND, ACE_OS::strlen(DIR_NOT_FIND), 0);
        return;
    }
    struct stat statbuf;
	stat(dir.c_str(), &statbuf);
	size_t filesize = statbuf.st_size;
    string res = "213 " + to_string(filesize)+ "\n";
    
    stream.send(res.c_str(), ACE_OS::strlen(res.c_str()), 0);
}
