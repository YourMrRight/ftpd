#include "Cwd_Command.h"
#include "User.h"
#include "Util.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

extern unordered_map<int, User*>handler_user;

void Cwd_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    User *current_user = handler_user[stream.get_handle()];
    string dir_cmd = cmd.substr(4,cmd.size());
    trim(dir_cmd);
    string upper_flag = "..";
    if (dir_cmd == upper_flag) {
        Command * cdup_cmd_obj = Command_Factory::getInstance()->get_cmd_obj("Cdup_Command");
        cdup_cmd_obj->exec(stream, cmd);
    } else {
        string toDir;
        if (!dir_cmd.empty()&&(dir_cmd[0] == '/')) {
            toDir = dir_cmd;
        } else {
            toDir.append(current_user->get_current_dir()).append(dir_cmd);
        }
        if (!(opendir(toDir.c_str()))) {
            //打开目录失败
            stream.send(DIR_NOT_FIND, ACE_OS::strlen(DIR_NOT_FIND), 0);
            return;
        } else {
            if (toDir[toDir.size()-1] != '/') {
                toDir.append("/");
            }
            current_user->set_current_dir(toDir);
            stream.send(CWD_SUCCESS,ACE_OS::strlen(CWD_SUCCESS));
            ACE_DEBUG((LM_DEBUG,"user %s change directory\n",current_user->get_user_name()));
        }
    }
}
