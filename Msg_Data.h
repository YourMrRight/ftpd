#ifndef __MSG_DATA_H__
#define __MSG_DATA_H__

#include <ace/Reactor.h>

#define MAX_BUFF_SIZE 1024

class Msg_Data
{
private:
    ACE_HANDLE *io_handle_;
    int data_flag_;

public:
    Msg_Data();
    ~Msg_Data();
    void set_data_flag(int flag);
    int get_data_flag() const;
    void set_io_handle(ACE_HANDLE *handle);
    ACE_HANDLE *get_io_handle() const;
    char data[MAX_BUFF_SIZE];
};

#endif // __MSG_DATA_H__