#ifndef __USER_H__
#define __USER_H__

#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>

#include <string>
#include <memory>

#define USR_INFO_SIZE 50
using namespace std;
class User
{
private:
	int usr_rights_;
	string current_dir_;
	string usr_name_;
	string usr_pwd_;
	int port_;
	string ip_;
	string rename_from_;
	bool is_pasv_;
public:
	User();
	~User();
	string get_pwd(string) const;
	void set_status(int);
	int get_status() const;
	string get_user_name() const;
	void set_user_name(string user_name);
	string get_current_dir() const;
	void set_current_dir(string dir);
	int get_port()const;
	void set_port(int p);
	string get_ip()const;
	void set_ip(string ip);
	string get_rename_from()const;
	void set_rename_from(string file_name);
	bool get_tran_pattern();
	void set_tran_pattern(bool pattern);\
	void init();
	shared_ptr<ACE_SOCK_Connector>data_conn_;
	shared_ptr<ACE_SOCK_Stream>data_stream_;
	shared_ptr<ACE_INET_Addr>inet_addr_;
	shared_ptr<ACE_SOCK_Acceptor>pasv_acceptor_;
};



#endif // __USER_H__