#include "Util.h"

#include <ace/OS_NS_sys_stat.h>

void Stringsplit(string str, const char split, vector<string> &res)
{
    istringstream iss(str);            // 输入流
    string token;                      // 接收缓冲区
    while (getline(iss, token, split)) // 以split为分隔符
    {
        res.push_back(token);
    }
}

void trim(string &s){
    if (!s.empty()) {
        s.erase(0, s.find_first_not_of(" "));
        if (string::npos != s.find_first_of("\n")) {
            s.erase(s.find_first_of("\n"), s.size());
        }
        if(string::npos != s.find_first_of("\r")){
            s.erase(s.find_first_of("\r"), s.size());
        }
    }
}

void trimToParentDir(string &s){
    if (!s.empty()) {
        s.erase(s.find_last_of("\n")-1,s.size());
    }
}

void power(int mode, char str[]){//权限解析
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if ((mode & S_IRUSR)) str[1] = 'r';
    if ((mode & S_IWUSR)) str[2] = 'w';
    if ((mode & S_IXUSR)) str[3] = 'x';
    if ((mode & S_IRGRP)) str[4] = 'r';
    if ((mode & S_IWGRP)) str[5] = 'w';
    if ((mode & S_IXGRP)) str[6] = 'x';
    if ((mode & S_IROTH)) str[7] = 'r';
    if ((mode & S_IWOTH)) str[8] = 'w';
    if ((mode & S_IXOTH)) str[9] = 'x';
}

int mk_dir(const char *dir)
{
	DIR *mydir = NULL;
	if ((mydir = opendir(dir)) == NULL){
		int ret = mkdir(dir, MODE); //创建目录
		if (ret != 0) {
			return -1;
		}
		printf("%s created sucess!/n", dir);
	} else {
		printf("%s exist!/n", dir);
        return -1;
	}

	return 0;
}

int rm_dir(std::string dir_full_path)
{    
    DIR* dirp = opendir(dir_full_path.c_str());    
    if(!dirp)
    {
        return -1;
    }
    struct dirent *dir;
    struct stat st;
    while((dir = readdir(dirp)) != NULL)
    {
        if(strcmp(dir->d_name,".") == 0
                || strcmp(dir->d_name,"..") == 0)
        {
            continue;
        }    
        std::string sub_path = dir_full_path + '/' + dir->d_name;
        if(lstat(sub_path.c_str(),&st) == -1)
        {
            continue;
        }    
        if(S_ISDIR(st.st_mode))
        {
            if(rm_dir(sub_path) == -1) 
            {
                closedir(dirp);
                return -1;
            }
            rmdir(sub_path.c_str());
        }
        else if(S_ISREG(st.st_mode))
        {
            unlink(sub_path.c_str());     
        }
        else
        {
            continue;
        }
    }
    if(rmdir(dir_full_path.c_str()) == -1)
    {
        closedir(dirp);
        return -1;
    }
    closedir(dirp);
    return 0;
}

void toUpper(std::string &str) {
    if (str.length() == 0) {
        return;
    }
    trim(str);
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    str[0] = std::toupper(str[0]);
    str.append("_Command");
}

bool file_exist(const char* full_file_path)
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
