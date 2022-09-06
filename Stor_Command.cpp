#include "Stor_Command.h"
#include "User.h"
#include "Util.h"
#include "Ftp_Msg.h"

extern unordered_map<int, User*>handler_user;

void Stor_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
    if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    User *current_user = handler_user[stream.get_handle()];
    stream.send(OPEN_ASCII_MODE, ACE_OS::strlen(OPEN_ASCII_MODE), 0);
    string f_name = cmd.substr(5,cmd.size());
    trim(f_name);
    int leng = f_name.size();

    string file_path(current_user->get_current_dir());
    file_path.append("/");
    file_path.append(f_name);
    char f_path[COMMAND_SIZE];
    ACE_OS::strcpy(f_path, file_path.c_str());

    FILE *fp;
    fp = ACE_OS::fopen(f_path, "wb+");

    if (!current_user->get_tran_pattern()){
        current_user->inet_addr_->set(current_user->get_port(), current_user->get_ip().c_str());
        if (current_user->data_conn_->connect(*(current_user->data_stream_), *(current_user->inet_addr_)) == -1){
            stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
            return;
        }
    }
    if (current_user->data_stream_ != nullptr) {
        char *rec_buf = new char[FILE_DATA_BLOCK];

        int rec_counter = current_user->data_stream_->recv(rec_buf, REC_BLOCK);

        if (rec_counter < REC_BLOCK)
            rec_buf[rec_counter] = 0;

        int total_count = rec_counter;

        while(rec_counter != -1 && rec_counter == REC_BLOCK) {
            rec_counter = current_user->data_stream_->recv(rec_buf, REC_BLOCK);
            ACE_OS::fwrite(rec_buf, 1, rec_counter, fp);
            total_count += rec_counter;
            if (rec_counter < REC_BLOCK) {
                break;
            }
        }
        rec_buf[total_count] = 0;

        ACE_OS::fclose(fp);
        current_user->data_stream_->close();

        stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
        delete[] rec_buf;
    }
}
