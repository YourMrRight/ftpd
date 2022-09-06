#include "Auth_Command.h"
#include "Ftp_Msg.h"


void Auth_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    stream.send(AUTH_CMD, ACE_OS::strlen(AUTH_CMD));
}
