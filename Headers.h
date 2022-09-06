#ifndef __HEADERS_H__
#define __HEADERS_H__

#include "User_Command.h"
#include "List_Command.h"
#include "Pass_Command.h"
#include "Pasv_Command.h"
#include "Syst_Command.h"
#include "Port_Command.h"
#include "Cwd_Command.h"
#include "Cdup_Command.h"
#include "Quit_Command.h"
#include "Retr_Command.h"
#include "Stor_Command.h"
#include "Xpwd_Command.h"
#include "Auth_Command.h"
#include "Pwd_Command.h"
#include "Size_Command.h"
#include "Mkd_Command.h"
#include "Rmd_Command.h"
#include "Rnfr_Command.h"
#include "Rnto_Command.h"
#include "Dele_Command.h"

REGISTER(Dele_Command);
REGISTER(Rnto_Command);
REGISTER(Rnfr_Command);
REGISTER(Rmd_Command);
REGISTER(Mkd_Command);
REGISTER(Size_Command);
REGISTER(Pwd_Command);
REGISTER(Auth_Command);
REGISTER(Xpwd_Command);
REGISTER(Stor_Command);
REGISTER(Retr_Command);
REGISTER(Quit_Command);
REGISTER(Cdup_Command);
REGISTER(Cwd_Command);
REGISTER(User_Command);
REGISTER(List_Command);
REGISTER(Pass_Command);
REGISTER(Syst_Command);
REGISTER(Pasv_Command);
REGISTER(Port_Command);
#endif // __HEADERS_H__