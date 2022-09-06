#ifndef __STOR_COMMAND_H__
#define __STOR_COMMAND_H__


#include "Command.h"
#include "Register_Action.h"

#define COMMAND_SIZE 80

class Stor_Command: public Command
{
public:
    virtual void exec(const ACE_SOCK_Stream &stream, const string &cmd) override;
};

#endif // __STOR_COMMAND_H__