#ifndef __INVOKER_H__
#define __INVOKER_H__


#include "Command.h"
class Invoker
{
private:
    Command *command;
public:
    void set_command(Command *cmd);
    void action(const ACE_SOCK_Stream &stream, const string &cmd);
};
#endif // __INVOKER_H__