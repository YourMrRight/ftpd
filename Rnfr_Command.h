#ifndef __RNFR_COMMAND_H__
#define __RNFR_COMMAND_H__

#include "Command.h"
#include "Register_Action.h"

class Rnfr_Command: public Command
{
public:
    virtual void exec(const ACE_SOCK_Stream &stream, const string &cmd) override;
};

#endif // __RNFR_COMMAND_H__