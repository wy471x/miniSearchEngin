#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<set>
#include<fstream>
using namespace std;
class Configuration
{
public:
    Configuration(const string& filepath);
    ~Configuration() {}
   map<string,string>& getConfigMap();//获取存放配置文件内容
   set<string> getStopWordList();//获取停用词集
private:
   string _filepath;//配置文件路径
   map<string,string> _configmap;//配置文件内容
   set<string> _stopwordlist;//停用词集
};

