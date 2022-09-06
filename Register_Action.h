#ifndef __REGISTER_ACTION_H__
#define __REGISTER_ACTION_H__

#include "Command_Factory.h"
#include <string>

using namespace std;

#define REGISTER(className) 											\
	className* objectCreator##className(){     							\
        return new className;                                         	\
    }                                                                  	\
    Register_Action g_creatorRegister##className(                        \
		#className,(PTRCreateObject)objectCreator##className)


class Register_Action {
public:
	Register_Action(string class_name,PTRCreateObject cmd_obj);
};

#endif // __REGISTER_ACTION_H__