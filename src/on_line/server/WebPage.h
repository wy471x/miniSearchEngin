#pragma once
#include"WordSegmentation.h"
#include"Configuration.h"
#include<cmath>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<sstream>
#include<utility>
#include<regex>
using namespace std;
struct comparator{
    bool operator()(const pair<string,int>& lhs,const pair<string,int>& rhs){
        return lhs.second > rhs.second;
    }
};
string ws2S(const wstring&);
wstring s2Ws(const string&);
class WebPage
{
public:
    WebPage(string& doc,Configuration& config,WordSegmentation& jieba);
    int getDocId();//获取文档的docid
    string getTitle();//获取文档的标题
    string getUrl();//获得链接
    string summary(const vector<string>& queryWords);//根据查询词生摘要内容
    map<string,int>& getWordsMap();//获取文档的词频统计map
    ~WebPage() {}

private:
    void processDoc(const string& doc,Configuration& config,WordSegmentation& jieba);//对格式化文档进行处理
    void calcTopK(vector<string>& wordsVec,int k,set<string>& stopWordList);//求取文档的topk词集
    friend bool operator ==(const WebPage& lhs,const WebPage& rhs);//判断文档是否相等
    friend bool operator <(const WebPage& lhs,const WebPage& rhs);//对文档按docid进行排序
    const static int TOPK_NUMBER = 10;
    int _docid;//文档id
    string _doc;
    string _doctitle;//文档标题
    string _docurl;//文档url
    string _doccontent;//文档内容
    string _docsummary;//文档摘要
    vector<string> _topWords;//词频最高的10
    map<string,int> _wordsmap;//保存每篇文档的所有词语和词频，去除了停用词
};
