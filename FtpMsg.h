#ifndef _FTP_MSG_H
#define _FTP_MSG_H

#pragma once


#define ESTABLISH   "220 connection  established.\n"
#define LOGIN_SUCCESS "230 Login successful.\n" //Remote system type is UNIX.\r\nUsing binary mode to transfer files.\r\n"

#define USR_LOGI_FAILED "530 Login incorrect.\n"
#define PWD_REQUIRE "331 Please specify the password.\n"
#define OPEN_ASCII_MODE "150 Opening ASCII mode data connection for current directory.\n"
#define PWD_REQUIRE "331 Password required for open localhost.\n"
#define TRANS_COMPELETE "226 Transfer complete.\n"
#define SYST_CMD "Remote system type is UNIX.\n"//Using binary mode to transfer files.\r\n"
#define USR_UNLOGIN "530 Please login with USER and PASS.\n"
#define CWD_SUCCESS "250 Directory successfully changed.\n"
#define DELETE_SUCCESS "250 Delete operation successful.\n"
#define DIR_NOT_FIND "550 The system can not find the file specified.\n"
#define OPEN_FILE_FAILED "425 File opening failed.\n"
#define DATA_CONN_FAILED "425 Cannot open data connection.\n"
#define PORT_COMMAND_SUCCESS "200 PORT command successful.\n"
#define AUTH_CMD "502 not impliment.\n"
#define PWD_CMD "257 "
#define MKDIR_CMD "257 "
#define PORT_EXHAUSTED "500 sorry! server port has been EXHAUSTED.\n"
#define PASV_CMD "227 start passive mode.\n"
#define MkDIR_ERRROE "550 Create directory operation failed.\n"
#define SIZE_CMD "213 ";
#define RNFR_CMD "350 Ready for RNTO.\n"
#define RNFR_FAILED "550 RNFR command failed.\n"
#define RNTO_FAILED "553 RNFR command failed.\n"
#define RENAME_SUCCESS "250 Rename successful.\n"
#define DELETE_FAILE "550 Delete operation failed.\n"


#define FILE_DATA_BLOCK 1024
#define REC_BLOCK 2048
#define BEGINPORT 10000
#define ENDPORT 15000

#define DEFAULT_DIR "/home/scutech/"
#define ROOT_DIR "/"

#define DEFAULT_IP "127,0,0,1"
#define DEFAULT_PORT 5051

#endif
