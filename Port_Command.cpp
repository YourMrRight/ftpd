#include "Port_Command.h"
#include "Util.h"
#include "User.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

#include "math.h"

extern unordered_map<int, User*>handler_user;

void Port_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    string ip_cmd = cmd.substr(5,cmd.size());
    trim(ip_cmd);
    string ip_addr = ip_cmd;

    int port1 = 0;
    int port2 = 0;
    int comma_counter = 0;
    int num_index = 0;
    int i = ip_addr.size()-1;

    while (!(comma_counter == 1 && ip_addr[i] == ',')) {
        if (ip_addr[i] == ',') {
            comma_counter++;
            num_index = 0;
        } else {
            double radix = 10;
            if (comma_counter == 0) {
                // the last segment of the ip address
                int curr_cha_number = ip_addr[i] - '0';

                port1 += int(curr_cha_number * pow(radix, num_index));

                num_index++;
            } else {
                int curr_cha_number = ip_addr[i] - '0';
                port2 += int(curr_cha_number * pow(radix, num_index));
                num_index++;
            }
        }
        i--;
    }

    int addr_len = ip_addr.size();
    for (int j = 0; j < addr_len; j++) {
        if (ip_addr[j] == ',') {
            ip_addr[j] = '.';
        }
    }
    ip_addr[i] = 0;
    handler_user[stream.get_handle()]->set_ip(ip_addr);
    int port = port2 * 256 + port1;
    handler_user[stream.get_handle()]->set_port(port);
    handler_user[stream.get_handle()]->set_tran_pattern(false);
    stream.send(PORT_COMMAND_SUCCESS, ACE_OS::strlen(PORT_COMMAND_SUCCESS), 0);
    ACE_DEBUG((LM_DEBUG, "user %s PORT command successful\n", handler_user[stream.get_handle()]->get_user_name()));
}
