#include "FilesCtr.h"

#include <ace/OS.h>
#include <sys/stat.h>
#include <ace/OS_NS_sys_stat.h>

FilesCtr::FilesCtr()
{
    this->file_data = new char[10000];    
}

FilesCtr::~FilesCtr()
{
    delete[] file_data;
}

bool FilesCtr::fileExist(const char* full_file_path)
{
    ACE_stat state;
    if(ACE_OS::stat(full_file_path, &state)<0)
    {
        return false;
    }
    if(S_IFDIR == (state.st_mode & S_IFMT))
    {
        return false;
    }
    return true;
}
