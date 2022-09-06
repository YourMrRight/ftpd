#include "User_Command.h"
#include "User.h"
#include "Ftp_Msg.h"
#include "ace/Log_Msg.h"
#include "Util.h"

#include <unordered_map>

using namespace std;

extern unordered_map<int, User*>handler_user;

void User_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    User *user = new User;
    string u_name = cmd.substr(5,cmd.size());
    trim(u_name);
    cout<<u_name<<endl;
    user->set_user_name(u_name);
    if (stream.send(PWD_REQUIRE, ACE_OS::strlen(PWD_REQUIRE), 0) == -1) {
        return;
    }   
    ACE_DEBUG((LM_DEBUG, "wait for password\n"));
    handler_user[stream.get_handle()] = user;
}
