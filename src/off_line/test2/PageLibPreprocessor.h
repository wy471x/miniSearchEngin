#pragma once
#include"Configuration.h"
#include"WordSegmentation.h"
#include"WebPage.h"
#include<cmath>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration& conf)
    :_conf(conf)
    {}
    void doProcess();//执行预处理
    ~PageLibPreprocessor() {}
    void shownewDoc();//测试用
    void shownewOffset();//测试用
private:
    void readInfoFromFile();//根据配置信息读取网页库和偏移库
    void cutRedundantPages();//取出冗余网页
    void buildInvertIndexTable();//创建倒排索引
    void storeOnDisk();//将经过预处理的网页库等进行存储
    Configuration& _conf;
    WordSegmentation _jieba;
    vector<WebPage> _pageLib;
    unordered_map<int,pair<int,int>> _offsetLib;
    unordered_map<string,vector<pair<int,double>>> _invertIndexTable;
};

