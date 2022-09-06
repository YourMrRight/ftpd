#ifndef __TASK_THREAD_H__
#define __TASK_THREAD_H__



#include <ace/Task.h>
#include <ace/SOCK_Stream.h>

#include <string>

using namespace std;

class Task_Thread : public ACE_Task<ACE_MT_SYNCH>
{
private:
    virtual int svc(void);
    int process_cmd(ACE_SOCK_Stream Stream, string cmd); 
    int is_user_login_valid(ACE_SOCK_STREAM &Stream);
};

#endif // __TASK_THREAD_H__