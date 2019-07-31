#include"DirScanner.h"
#include"PageLib.h"
#include<memory>
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
void test0(){
    Configuration conf("/home/dunk/miniSearchEngin/conf/conf");
    map<string,string> _map = conf.getConfigMap();
    set<string> _stopwords = conf.getStopWordList();
    for(auto& i : _stopwords){
       cout<<i<<" ";
    }
    cout<<endl;
}
void test1(){
   Configuration conf("/home/dunk/miniSearchEngin/conf/conf");
   map<string,string> _map = conf.getConfigMap();
   DirScanner dirscanner(conf);
   dirscanner("xml");
   for(auto& i : dirscanner.files()){
       cout<<i<<endl;
   }
}
void test2(){
   Configuration conf("/home/dunk/miniSearchEngin/conf/conf");
   DirScanner dirscanner(conf);
   PageLib pagelib(conf,dirscanner);
   pagelib.create();
  // pagelib.showpagelib();
   pagelib.store();
}
int main()
{
    //test0();
    //test1();
    test2();
    return 0;
}

