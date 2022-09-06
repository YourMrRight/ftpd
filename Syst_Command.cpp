
#include "Syst_Command.h"
#include "Ftp_Msg.h"



void Syst_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{   
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    if (stream.send(SYST_CMD, ACE_OS::strlen(SYST_CMD)) ==-1) {
        return;
    }
}

