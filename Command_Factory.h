#ifndef __COMMAND_FACTORY_H__
#define __COMMAND_FACTORY_H__

#include "Command.h"

#include <string>
#include <unordered_map>
using namespace std;
typedef Command* (*PTRCreateObject)(void); 

class Command_Factory
{
private:
    unordered_map<string, PTRCreateObject>command_map_;
    unordered_map<string,Command*>cmd_obj_pool_;
    Command_Factory();
    static Command_Factory *instance;
public:
    Command* get_cmd_obj(string class_name);  
    void regist_class(string name, PTRCreateObject obj) ;  
    static Command_Factory* getInstance() ;  
};

#endif // __COMMAND_FACTORY_H__