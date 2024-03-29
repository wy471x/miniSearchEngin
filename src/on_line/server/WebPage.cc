#include"WebPage.h"
wstring s2Ws(const string& str){
    if(str.empty()){
        return L"";
    }
    unsigned len = str.size() + 1;
    setlocale(LC_CTYPE,"en_US.UTF-8");
    unique_ptr<wchar_t[]> p(new wchar_t[len]);
    mbstowcs(p.get(),str.c_str(),len);
    wstring w_str(p.get());
    return w_str;
}
string ws2S(const wstring& wstr){
    if(wstr.empty())
        return "";
    unsigned len = wstr.size() * 4 + 1;
    setlocale(LC_CTYPE,"en_US.UTF-8");
    unique_ptr<char[]> p(new char[len]);
    wcstombs(p.get(),wstr.c_str(),len);
    string str(p.get());
    return str;
}
WebPage::WebPage(string& doc,Configuration& conf,WordSegmentation& jieba){
    processDoc(doc,conf,jieba);
    set<string> stopwordlist = conf.getStopWordList();
    calcTopK(_topWords,TOPK_NUMBER,stopwordlist);
}
//获取文档编号
int WebPage::getDocId(){
    return _docid;
}
//获取文档
string WebPage::getTitle(){
    return _doctitle;
}
//获取链接
string WebPage::getUrl(){
   return _docurl;
}
//获取词频map
map<string,int>& WebPage::getWordsMap(){
    return _wordsmap;
}
//根据查询词生成文档的摘要
string WebPage::summary(const vector<string>& queryWords){
       map<int,string,greater<int>> idx;//每个查询词在文档中的下标
       for(auto& i : queryWords){
           if(_doccontent.find(i) < _doccontent.size()){
               idx.insert(make_pair(_doccontent.find(i),i));
           }
       }
       size_t i;
       if((i = _doccontent.find("。",idx.begin()->first)) < _doccontent.size())
       {
             _docsummary = _doccontent.substr(0,i);
       }else if((i = _doccontent.find(".",idx.begin()->first)) < _doccontent.size())
       {
             _docsummary = _doccontent.substr(0,i);
       }else {
           _docsummary = _doccontent.substr(0,idx.begin()->first + idx.begin()->second.size());
       }
    return _docsummary;
}
//文档处理
void WebPage::processDoc(const string& doc,Configuration& conf,WordSegmentation& jieba){
        _doc = doc;
        size_t r = 0,l = 0;
        if( (l = doc.find("<docid>"))&&(r = doc.find("</docid>"))){
            size_t size = r - l - 7;
            _docid = stoi(doc.substr(l + 7,size));
        }
        if( (l = doc.find("<title>"))&& (r = doc.find("</title>"))){
            size_t size = r - l - 7;
            _doctitle = doc.substr(l + 7,size);
        }
        if( (l = doc.find("<link>"))&& (r = doc.find("</link>"))){
            size_t size = r - l - 6;
            _docurl = doc.substr(l + 6,size);
        }
        if( (l = doc.find("<content>"))&& (r = doc.find("</content>"))){
            size_t size = r - l - 9;
            _doccontent = doc.substr(l + 9,size);
        }
    wstring wstr = s2Ws(_doctitle);
    vector<string> str;
    wregex label(L"[^\u4E00-\u9FA5a-zA-Z]");
    wstr = regex_replace(wstr,label,L" ");
    str = jieba.cut(ws2S(wstr));
    for(auto& i : str){
       if( i != " " ){ 
          _wordsmap[i]++;
       }
    }
    wstr = s2Ws(_doccontent);
    wstr = regex_replace(wstr,label,L" ");
    str = jieba.cut(ws2S(wstr));
    for(auto& i: str){
       if(i != " "){ 
          _wordsmap[i]++;
       }
    }
}
//求取文档的top词集
void WebPage::calcTopK(vector<string>& wordsVec,int k,set<string>& stopWordList){
          vector<pair<string,int>> ispv(_wordsmap.begin(),_wordsmap.end());
          for(auto& i : ispv){
              if(stopWordList.find(i.first)!= stopWordList.end())
                      stopWordList.erase(stopWordList.find(i.first));  
          }
          sort(ispv.begin(),ispv.end(),comparator());
          int size = ispv.size();
          if( size >= k ){
              for(auto i = 0; i < k;++i){
                  wordsVec.push_back(ispv[i].first);
               } 
          }
          else {
               for(int i = 0; i < size;i++){
                  wordsVec.push_back(ispv[i].first);
              }
          }
}
// ==
bool operator==(const WebPage& lhs,const WebPage& rhs){
    int n = 0;
    for(auto i = lhs._topWords.begin(); i != lhs._topWords.end(); ++i){
        for(auto j = rhs._topWords.begin(); j != rhs._topWords.end();++j){
            if( *i == *j ){
               n++;
            }
        }
    }
    if( 6 == n ){
        return true;
    }
    return false;
}
// <
bool operator<(const WebPage& lhs,const WebPage& rhs){
    if(lhs._docid < rhs._docid){
        return true;
    }
    return false;
}
