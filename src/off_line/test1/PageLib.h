#pragma once
#include"Configuration.h"
#include"DirScanner.h"
#include"tinyxml2.h"
#include<regex>
#include<iostream>
#include<vector>
#include<map>
using namespace tinyxml2;
using namespace std;
struct RssItem{
    string title;
    string link;
    string description;
    string content;
};
class PageLib
{
public:
    PageLib(Configuration& conf,DirScanner& dirscanner)
    :_conf(conf)
     ,_dirscanner(dirscanner)
    {}
    void create();//创建网页库
    void store();//保存网页库与网页偏移库
    void showpagelib();//用于测试
    ~PageLib() {}
private:
   Configuration& _conf;//配置文件对象
   DirScanner& _dirscanner;//目录扫描类
   vector<string> _vecfiles;//存放格式化的文件
   map<int,pair<int,int>> _offsetlib;//网页偏移库
};

