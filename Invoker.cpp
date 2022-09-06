#include "Invoker.h"


void Invoker::set_command(Command *cmd)
{
    this->command = cmd;
}

void Invoker::action(const ACE_SOCK_Stream &stream, const string &cmd)
{
    this->command->exec(stream, cmd);
}
