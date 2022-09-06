#include "Mkd_Command.h"
#include "User.h"
#include "Util.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*>handler_user;

void Mkd_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    
    User *current_user = handler_user[stream.get_handle()];
    string new_dir = cmd.substr(4,cmd.size());
    trim(new_dir);
    if (new_dir[0] != '/' && new_dir.size() > 1) {
        new_dir = current_user->get_current_dir() + new_dir;
    }
    if (mk_dir(new_dir.c_str())==-1) {
        stream.send(MkDIR_ERRROE, ACE_OS::strlen(MkDIR_ERRROE), 0);
        return;
    }
    string resMessage = MKDIR_CMD + new_dir +" created.\n";
    stream.send(resMessage.c_str(), resMessage.size(), 0);
}
