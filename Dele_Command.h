#ifndef __DELE_COMMAND_H__
#define __DELE_COMMAND_H__

#include "Command.h"
#include "Register_Action.h"

class Dele_Command: public Command
{
public:
    virtual void exec(const ACE_SOCK_Stream &stream, const string &cmd) override;
};

#endif // __DELE_COMMAND_H__