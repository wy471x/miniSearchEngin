#pragma once
#include"Configuration.h"
#include"WordSegmentation.h"
#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
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
    WebPage(string& doc,Configuration& conf,WordSegmentation& jieba);
    int getDocid();//获取文档ID
    string getDoc();//获取文档
    map<string,int>& getWordsMap();//获取词频map
    ~WebPage() {}
    
private:
 void processDoc(const string& doc,Configuration& conf);
 void calcTopK(vector<string>& wordsVec,int k,set<string>& stopWordList);//求取文档的前10个词集
 const static int TOPK_NUMBER = 10;
 string _doc;//整个文档
 string _doctitle;//文档标题
 int  _docid;//文档编号
 string _doclink;//文档连接
 string _doccontent;//文档内容
 vector<string> _topWords;//词频最高的前1()0个单词
 map<string,int> _wordsmap;//保存每篇文档的所有单词和词频，不包括停用词
 friend bool operator==(const WebPage& lhs,const WebPage& rhs);//判断两篇文档是否相等
 friend bool operator<(const WebPage& lhs,const WebPage& rhs);//对文档进行排序
};

