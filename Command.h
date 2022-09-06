#ifndef __COMMAND_H__
#define __COMMAND_H__



#include<ace/SOCK_Stream.h>

#include <iostream>
#include <string>

using namespace std;
class Command{
public:
    virtual void exec(const ACE_SOCK_Stream &stream, const string &cmd) = 0;
    virtual bool is_user_login(const ACE_SOCK_Stream &stream);
};


#endif // __COMMAND_H__