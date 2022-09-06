#include "Retr_Command.h"
#include "User.h"
#include "Util.h"
#include "Ftp_Msg.h"

#include <ace/Log_Msg.h>

extern unordered_map<int, User*>handler_user;

void Retr_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    User *current_user = handler_user[stream.get_handle()];
    string file_name = current_user->get_current_dir();

    file_name.append(cmd.substr(5,cmd.size()));
    trim(file_name);
    int len = file_name.size();

    if (!file_exist(file_name.c_str())) {
        stream.send(DIR_NOT_FIND, ACE_OS::strlen(DIR_NOT_FIND), 0);
        ACE_DEBUG((LM_DEBUG,"user %s requested for an invalid file\n",current_user->get_user_name()));
        return;
    } else {
        //transfer file
        if (!current_user->get_tran_pattern()) {
            current_user->inet_addr_->set(current_user->get_port(), current_user->get_ip().c_str());
            if (current_user->data_conn_->connect(*(current_user->data_stream_), *(current_user->inet_addr_)) == -1){
                stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
                return;
            }
        }
        if (current_user->data_stream_ != nullptr) {
            FILE *fp;
            fp = ACE_OS::fopen(file_name.c_str(), "rb");
            if (fp == nullptr) {
                stream.send(OPEN_FILE_FAILED, ACE_OS::strlen(OPEN_FILE_FAILED), 0);
                ACE_DEBUG((LM_DEBUG,"user %s open file failed\n",current_user->get_user_name()));
                return;
            }

            ACE_OS::fseek(fp, 0L, 2);
            int len = ACE_OS::ftell(fp);
            ACE_OS::fseek(fp, 0L, 0);

            char *file_block = new char[FILE_DATA_BLOCK];
            int read_len = ACE_OS::fread(file_block, 1, FILE_DATA_BLOCK, fp);
            file_block[read_len] = 0;
            int data_read = 1024;
            ACE_DEBUG((LM_DEBUG, "opening file: %s\n", file_name));
            std::string str("150 Opening ASCII mode data connection for ");
            str.append(file_name);
            str.append(".\n");
            stream.send(str.c_str(),ACE_OS::strlen(str.c_str()),0);
            current_user->data_stream_->send(file_block,ACE_OS::strlen(file_block), 0);
            while(data_read<len) {
                read_len = ACE_OS::fread(file_block, 1, FILE_DATA_BLOCK, fp);
                data_read += read_len;
                current_user->data_stream_->send(file_block, read_len, 0);
            }
            current_user->data_stream_->close();
            stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
            ACE_OS::fclose(fp);
            delete[] file_block;
            ACE_DEBUG((LM_DEBUG,"user %s download file %s.\n",current_user->get_user_name(), file_name));
        }
    }
}
