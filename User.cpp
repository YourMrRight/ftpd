#include "User.h"

#include <ace/OS.h>

#include <fstream>
#include <iostream>
#include <string.h>

User::User()
{
	usr_rights_ = 0;
	port = USE_DEFAULT_PORT;
	current_dir_ = DEFAULT_DIR;
	port = DEFAULT_PORT;
	ip = DEFAULT_IP;
}

User::~User()
{
}

string User::getpwd(string u_name)const
{
	std::ifstream ifst;
	ifst.open("Userinfo.txt");
	char usrname[USR_INFO_SIZE];
	char *usrpwd = new char[USR_INFO_SIZE];
	int exist = 1;
	while (ifst.getline(usrname, USR_INFO_SIZE))
	{
		if (u_name == string(usrname))
		{
			ifst.getline(usrpwd, USR_INFO_SIZE);
			exist = 0;
			break;
		}
	}
	if (exist)
	{
		usrpwd = nullptr;
	}
	return string((usrpwd==nullptr?"":usrpwd));
}

void User::setStatus(int stat)
{
	this->usr_rights_ = stat;
}

int User::getStatus()const
{
	return this->usr_rights_;
}

string User::getUserName() const
{
	return this->usr_name_;
}

void User::setUserName(string name)
{
	this->usr_name_ = name;
}

string User::getCurrentDir() const
{
	return this->current_dir_;
}

void User::setCurrentDir(string dir)
{
	this->current_dir_ = dir;
}

int User::getPort() const
{
	return this->port;	
}

void User::setPort(int p)
{
	this->port = p;
}

string User::getIP() const
{
	return this->ip;
}

void User::setIP(string IP)
{
	this->ip = IP;
}
string User::getRenameFrom() const
{
	return rename_from_;
}

void User::setRenamefrom(string filename)
{
	this->rename_from_ = filename;
}