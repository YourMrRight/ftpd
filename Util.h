#ifndef UTIL_H_
#define UTIL_H_

#pragma once

#include <vector>
#include <string>
#include <sstream>

#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>//memcmp()
#include<grp.h>//getgrgid
#include<unistd.h>//getcwd
#include<pwd.h>//getpwuid
#include<cstdio>
using namespace std;

#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)
#define LS_CMD "ls"
#define CD_CMD "cd"

char filename[100][255];
int filenum = 0;

using namespace std;

typedef uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
hash_t hash_(char const *str)
{
    hash_t ret{basis};
    while (*str)
    {
        /* code */
        ret ^= *str;
        ret *= prime;
        str++;
    }
    return ret;
}
constexpr hash_t hash_compile_time(char const *str, hash_t last_value = basis)
{
    return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}

constexpr unsigned long long operator"" _hash(char const *p, size_t)
{
    return hash_compile_time(p);
}

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
    if (!s.empty()){
        s.erase(0, s.find_first_not_of(" "));
    }
    if(!s.empty()){
        s.erase(s.find_first_of("\n"), s.size());
    }
}
void trimToParentDir(string &s){
    if(!s.empty()){
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
	if ((mydir = opendir(dir)) == NULL) //判断目录
	{
		int ret = mkdir(dir, MODE); //创建目录
		if (ret != 0)
		{
			return -1;
		}
		printf("%s created sucess!/n", dir);
	}
	else
	{
		printf("%s exist!/n", dir);
        return -1;
	}

	return 0;
}
#endif