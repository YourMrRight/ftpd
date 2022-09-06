#ifndef USER_H_
#define USER_H_
#pragma once



#define USR_INFO_SIZE 50
#define USE_DEFAULT_PORT 20
#define USER_DEFAULT_IP "127.0.0.1"
using namespace std;
class User
{
private:
	int usr_rights_;
	string current_dir_;
	string usr_name_;
	string usr_pwd_;
	int port;
	string ip;
	string rename_from_;
public:
	User();
	~User();
	string getpwd(string) const;
	void setStatus(int);
	int getStatus() const;
	string getUserName() const;
	void setUserName(string username);
	string getCurrentDir() const;
	void setCurrentDir(string dir);
	int getPort()const;
	void setPort(int p);
	string getIP()const;
	void setIP(string IP);
	string getRenameFrom()const;
	void setRenamefrom(string filename);
};

#endif