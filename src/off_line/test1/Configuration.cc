#include"Configuration.h"
Configuration::Configuration(const string& filepath){
    _filepath = filepath;
}
//获取配置文件内容
map<string,string>& Configuration::getConfigMap(){
    ifstream ifs(_filepath);
    string line;
    string name_,path_;
    while(ifs>>name_>>path_){
       _configmap.insert(make_pair(name_,path_));
    }
    return _configmap;
}
//获取停用词词集
set<string> Configuration::getStopWordList(){
       ifstream ifs_en(_configmap["stop_words_en"]);
       string word;
       while(ifs_en>>word){
           _stopwordlist.insert(word);
       }
       ifs_en.close();
       ifstream ifs_cn(_configmap["stop_words_cn"]);
       while(ifs_cn>>word){
           _stopwordlist.insert(word);
       }
       ifs_cn.close();
       return _stopwordlist;
}
