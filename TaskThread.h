#ifndef TASKTHREAD_H_
#define TASKTHREAD_H_
#pragma once

#include <ace/Task.h>
#include <ace/SOCK_Stream.h>

#include <string>

using namespace std;

class TaskThread : public ACE_Task<ACE_MT_SYNCH>
{
    virtual int svc(void);
    int processCmd(ACE_SOCK_Stream Stream, string cmd); 
    int isUserLoginValid(ACE_SOCK_STREAM &Stream);
public:
    int processUSER(ACE_SOCK_STREAM &Stream, string &cmd);
    int processPASS(ACE_SOCK_Stream &Stream, string &cmd);
    int processSYST(ACE_SOCK_Stream &Stream, string &cmd);
    int processPASV(ACE_SOCK_Stream &Stream, string &cmd);
    int processPORT(ACE_SOCK_Stream &Stream, string &cmd);
    int processCWD(ACE_SOCK_Stream &Stream, string &cmd);
    int processQUIT(ACE_SOCK_Stream &Stream);
    int processRETR(ACE_SOCK_Stream &Stream, string &cmd);
    int processLIST(ACE_SOCK_Stream &Stream, string &cmd);
    int processSTOR(ACE_SOCK_Stream &Stream, string &cmd);
    int processXPWD(ACE_SOCK_Stream &Stream, string &cmd);
    int processAUTH(ACE_SOCK_Stream &Stream, string &cmd);
    int processPWD(ACE_SOCK_Stream &Stream, string &cmd);
    int processSIZE(ACE_SOCK_Stream &Stream, string &cmd);
    int processCDUP(ACE_SOCK_Stream &Stream);
    int processMKD(ACE_SOCK_Stream &Stream, string &cmd);
    int processRMD(ACE_SOCK_Stream &Stream, string &cmd);
    int processRNFR(ACE_SOCK_Stream &Stream, string &cmd);
    int processRNTO(ACE_SOCK_Stream &Stream, string &cmd);
    int processDELE(ACE_SOCK_Stream &Stream, string &cmd);
};

#endif