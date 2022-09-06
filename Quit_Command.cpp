#include "Quit_Command.h"
#include "User.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*>handler_user;

void Quit_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    string q_cmd = "221 Goodbye\n";
    if(stream.send(q_cmd.c_str(), ACE_OS::strlen(q_cmd.c_str()), 0) ==-1){
        return;
    }
    if (handler_user.find(stream.get_handle()) != handler_user.end()) {
        User *del = handler_user[stream.get_handle()];
        handler_user.erase(stream.get_handle());
        delete del;
    }
}
