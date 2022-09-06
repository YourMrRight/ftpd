#ifndef __RNTO_COMMAND_H__
#define __RNTO_COMMAND_H__


#include "Command.h"
#include "Register_Action.h"

class Rnto_Command: public Command
{
public:
    virtual void exec(const ACE_SOCK_Stream &stream, const string &cmd)override;
};


#endif // __RNTO_COMMAND_H__