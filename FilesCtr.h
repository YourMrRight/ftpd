#ifndef FILE_H_
#define FILE_H_
#define BUFFER_SIZE 1024
#define FILE_OPEN_FAILED "425 File opening failed. \n"

class FilesCtr
{
private:
    char* file_data;
public:
    FilesCtr();
    ~FilesCtr();
    bool fileExist(const char*);
};

#endif