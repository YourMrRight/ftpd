#include "Msg_Data.h"

Msg_Data::Msg_Data(): io_handle_(nullptr), data_flag_(-1)
{
    ACE_OS::memset(data, 0, sizeof(data));
}

Msg_Data::~Msg_Data()
{
}

void Msg_Data::set_data_flag(int flag)
{
    this->data_flag_ = flag;
}

int Msg_Data::get_data_flag() const
{
    return this->data_flag_;
}

void Msg_Data::set_io_handle(ACE_HANDLE *handle)
{
    this->io_handle_ = handle;
}   

ACE_HANDLE *Msg_Data::get_io_handle() const
{
    return io_handle_;
}
