#include "Register_Action.h"

Register_Action::Register_Action(string class_name, PTRCreateObject cmd_obj)
{
    Command_Factory::getInstance()->regist_class(class_name, cmd_obj);
}
