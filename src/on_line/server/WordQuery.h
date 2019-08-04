#pragma once
#include "RedisTool.h"
#include "Configuration.h"
#include "WebPage.h"
#include "WordSegmentation.h"
#include<cmath>
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<json/json.h>
using namespace std;
using namespace Json;
class WordQuery
{
public:
    WordQuery(Configuration& conf):_conf(conf){}
    string doQuery(const string& str);

    ~WordQuery() {}

private:
    void loadLibrary();
    vector<double> getQueryWordsWeightVector(const vector<string>& queryWords);//计算查询词的权重值
    bool executeQuery(const vector<string>& queryWords,vector<pair<int,vector<double>>>& resultVec);//执行查询
    string  createJson(vector<int>& docIdVec,const vector<string>& queryWords);
    string returnNoAnswer();
    Configuration& _conf;
    WordSegmentation _jieba;
    vector<WebPage> _pagelib;//网页库
    unordered_map<int,pair<int,int>> _offsetlib;//偏移库
    unordered_map<string,set<pair<int,double>>> _invertIndexTable;//倒排索引表
};

