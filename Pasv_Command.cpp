#include "Pasv_Command.h"
#include "User.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

extern unordered_map<int, User*> handler_user;

void Pasv_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    User* user = handler_user[stream.get_handle()];
    int pPort;
    for(pPort = BEGINPORT; pPort < ENDPORT; pPort++) {
        if(user->inet_addr_->set(pPort) == -1) {
            continue;
        }
        if(user->pasv_acceptor_->open(*(user->inet_addr_))==-1) {
            continue;
        }
        break;
    }
    if (pPort >= ENDPORT) {
        stream.send(PORT_EXHAUSTED, ACE_OS::strlen(PORT_EXHAUSTED));
    }
    std::stringstream ipAndPort;
    ipAndPort<<PASV_CMD<<"("<<DEFAULT_IP<<","<<(pPort>>8)<<","<<(pPort & 0xFF)<<").\n";
    cout<<ipAndPort.str()<<endl;
    stream.send(ipAndPort.str().c_str(), ACE_OS::strlen(ipAndPort.str().c_str()),0);
    if (user->pasv_acceptor_->accept(*(user->data_stream_)) == -1) {
        ACE_DEBUG((LM_DEBUG, "监听失败！\n"));
    }
    user->set_tran_pattern(true);
}
