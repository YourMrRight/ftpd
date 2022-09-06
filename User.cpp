#include "User.h"
#include "Ftp_Msg.h"


#include <ace/OS.h>

#include <fstream>
#include <iostream>
#include <string.h>

User::User(): usr_rights_(0), current_dir_(DEFAULT_DIR), 
	port_(DEFAULT_PORT), ip_(DEFAULT_IP)
{
}

User::~User()
{
}

string User::get_pwd(string u_name)const
{
	std::ifstream ifst;
	std::string path = __FILE__;
	path = path.substr(0,path.find_last_of("/"));
	path.append("/Userinfo.txt");
	cout<<path<<endl;
	ifst.open(path);
	char usrname[USR_INFO_SIZE];
	char *usrpwd = new char[USR_INFO_SIZE];
	int exist = 1;
	while (ifst.getline(usrname, USR_INFO_SIZE)) {
		if (u_name == string(usrname)) {
			ifst.getline(usrpwd, USR_INFO_SIZE);
			exist = 0;
			break;
		}
	}
	if (exist) {
		usrpwd = nullptr;
	}
	return string((usrpwd==nullptr?"":usrpwd));
}

void User::set_status(int stat)
{
	this->usr_rights_ = stat;
}

int User::get_status()const
{
	return this->usr_rights_;
}

string User::get_user_name() const
{
	return this->usr_name_;
}

void User::set_user_name(string name)
{
	this->usr_name_ = name;
}

string User::get_current_dir() const
{
	return this->current_dir_;
}

void User::set_current_dir(string dir)
{
	this->current_dir_ = dir;
}

int User::get_port() const
{
	return this->port_;	
}

void User::set_port(int p)
{
	this->port_ = p;
}

string User::get_ip() const
{
	return this->ip_;
}

void User::set_ip(string IP)
{
	this->ip_ = IP;
}
string User::get_rename_from() const
{
	return rename_from_;
}

void User::set_rename_from(string filename)
{
	this->rename_from_ = filename;
}

bool User::get_tran_pattern()
{
	return this->is_pasv_;
}

void User::set_tran_pattern(bool pattern)
{
	this->is_pasv_ = pattern;
}

void User::init()
{
	data_conn_.reset(new ACE_SOCK_Connector());
	data_stream_.reset(new ACE_SOCK_Stream());
	inet_addr_.reset(new ACE_INET_Addr());
	pasv_acceptor_.reset(new ACE_SOCK_Acceptor());
}
