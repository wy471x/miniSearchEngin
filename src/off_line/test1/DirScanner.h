#pragma once
#include"Configuration.h"
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include<vector>
#include<string>
using namespace std;
class DirScanner
{
public:
    DirScanner(Configuration& conf);
    void operator()(const string& dirname);//重载运算符调用traverse
    vector<string> files();//返回_vecFilesfiles;
    void traverse(const string& dirname);//获取某一目录下的所有文件
    ~DirScanner() {}
   
private:
   vector<string> _vecFilesfiles;//存放每个语料的绝对路径
   Configuration& _conf;//配置文件对象的引用
};

