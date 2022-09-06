#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>
#include <sstream>
#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<grp.h>
#include<unistd.h>
#include<pwd.h>
#include<cstdio>
#include <algorithm>
using namespace std;

#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)


void Stringsplit(string str, const char split, vector<string> &res);

void trim(string &s);

void trimToParentDir(string &s);

void power(int mode, char str[]);

int mk_dir(const char *dir);

int rm_dir(std::string dir_full_path);

void toUpper(std::string &str);

bool file_exist(const char* full_file_path);

#endif // __UTIL_H__