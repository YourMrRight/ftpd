#include "List_Command.h"
#include "Util.h"
#include "User.h"
#include "Ftp_Msg.h"
#include <unordered_map>

using namespace std;

extern unordered_map<int, User *> handler_user;

// void List_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
// {

// }

void List_Command::exec(const ACE_SOCK_Stream &stream, const string &cmd)
{
        if (!is_user_login(stream)) {
        stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));        
        return;
    }
    stream.send(OPEN_ASCII_MODE, ACE_OS::strlen(OPEN_ASCII_MODE), 0);
    User *current_user = handler_user[stream.get_handle()];

    if (!current_user->get_tran_pattern()) {
        current_user->inet_addr_->set(current_user->get_port(), current_user->get_ip().c_str());
        if (current_user->data_conn_->connect(*(current_user->data_stream_), *(current_user->inet_addr_)) == -1){
            stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
            return;
        }
    }
    if (current_user->data_stream_ != nullptr) {
        DIR *dir;
        struct dirent *file;
        struct stat info;
        if (!(dir = opendir(current_user->get_current_dir().c_str()))) {
            //打开目录失败
            cout<<"fail to read"<<current_user->get_current_dir().c_str();
            return;
        }
        struct passwd *userinfo;
        struct group *groupinfo;
        while((file = readdir(dir))!= NULL) {
            //文件无文件名只有后缀
            if(strncmp(file->d_name, ".", 1) == 0)
                continue;
            //strcpy(filename[filenum++],file->d_name);
            //获取文件信息，存储在info中
            stat(file->d_name,&info); 
            userinfo = getpwuid(info.st_uid);//所有者用户名 
            groupinfo = getgrgid(info.st_gid);//所有者所在组用户名
            char pw[11];
            power(info.st_mode,pw);//文件权限信息
            string file_info;
            long long fz = info.st_size;
            file_info.append(pw).append(" ").append((userinfo != nullptr ? userinfo->pw_name: ""))
                .append("  ").append(groupinfo != nullptr ?groupinfo->gr_name : "").append("  ");
            stringstream ss;
            ss<<fz<<" ";
            string date_str = 4 + ctime(&info.st_mtime);
            trim(date_str);
            ss<<date_str;
            file_info.append(ss.str());
            file_info.append(" ").append(file->d_name).append("\n");
            current_user->data_stream_->send(file_info.c_str(),file_info.size(),0);
        }
        closedir(dir);
        current_user->data_stream_->close();
        stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
    }
}
