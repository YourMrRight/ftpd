
#include "Command_Factory.h"

Command_Factory *Command_Factory::instance = nullptr;
Command_Factory::Command_Factory()
{
}

Command *Command_Factory::get_cmd_obj(string class_name)
{
    unordered_map<string, Command*>::const_iterator obj_iter;
    obj_iter = cmd_obj_pool_.find(class_name);
    if (obj_iter != cmd_obj_pool_.end()) {
        return obj_iter->second;
    }
    unordered_map<string, PTRCreateObject>::const_iterator iter;
    iter = command_map_.find(class_name);
    if (iter == command_map_.end()) {
        return nullptr;
    } else {
        Command *obj= iter->second();
        cmd_obj_pool_.insert(unordered_map<string, Command*>::value_type(class_name,obj));
        return obj;
    }
}

void Command_Factory::regist_class(string name, PTRCreateObject obj)
{
    command_map_.insert(pair<string, PTRCreateObject>(name, obj)) ;  
}

Command_Factory *Command_Factory::getInstance()
{
    if (instance == nullptr){
        instance = new Command_Factory();
    }
    return instance;
}
