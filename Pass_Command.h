#ifndef __PASS_COMMAND_H__
#define __PASS_COMMAND_H__

#include "Command.h"
#include "Util.h"
#include "Register_Action.h"

class Pass_Command: public Command
{
public:
    virtual void exec(const ACE_SOCK_STREAM &stream, const string &cmd) override;
};


#endif // __PASS_COMMAND_H__