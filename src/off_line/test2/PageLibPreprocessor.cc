#include"PageLibPreprocessor.h"
void PageLibPreprocessor::doProcess(){
     readInfoFromFile();
     cutRedundantPages();
     storeOnDisk();
}
//读取配置信息
void PageLibPreprocessor::readInfoFromFile(){
     ifstream ifs_page(_conf.getConfigMap()["ripepage.lib"]); 
     string line;
     string alldoc;
     while(ifs_page>>line){
      alldoc += line;
     }
    //文档库，以下部分是为了获取每一个文档
    size_t i = 0, j;
    string doc ;
    ofstream ofs("pagelib");
    while((j = alldoc.find("</doc>",i)) < alldoc.size()){
        int size = j + 6 - i;//每个文档的大小
        doc = alldoc.substr(i,size);
        ofs<<doc<<endl;
        WebPage webpage(doc,_conf,_jieba);
        _pageLib.push_back(webpage);
        i = j + 6;
    }
}
//去除冗余网页
void PageLibPreprocessor::cutRedundantPages(){
    for(auto i = _pageLib.begin(); i != _pageLib.end(); ++i){
        for(auto j = i + 1; j != _pageLib.end(); ++j){
            if(*i == *j){
                if((*i) < (*j)){
                   _pageLib.erase(j);
                }
            }
        }
    }       
}
//创建倒排索引
void PageLibPreprocessor::buildInvertIndexTable(){

}
//存储处理之后的网页库与偏移库及倒排索引表
void PageLibPreprocessor::storeOnDisk(){
     ofstream ofs_newpage(_conf.getConfigMap()["newripepage.lib"]);
     ofstream ofs_newoffset(_conf.getConfigMap()["newoffset.lib"]);
     for(auto& i : _pageLib){
         string tempdoc = i.getDoc();
         size_t l = 0, r = 0;
         if((l = tempdoc.find("<doc>"))!= tempdoc.size()){
             ofs_newpage<<tempdoc.substr(l,5)<<endl;;
         }
         if((l = tempdoc.find("<docid>"))&&(r = tempdoc.find("</docid>"))){
            ofs_newpage<< "    "+ tempdoc.substr(l,r - l + 8)<<endl;
         }
         if((l = tempdoc.find("<title>"))&&(r = tempdoc.find("</title>"))){
             ofs_newpage<< "    " + tempdoc.substr(l,r- l + 8)<<endl;
         }
         if((l = tempdoc.find("<link>"))&&( r = tempdoc.find("</link>"))){
             ofs_newpage<<"    " + tempdoc.substr(l,r - l + 8)<<endl;
         }
         if((l = tempdoc.find("<content>"))&&( r = tempdoc.find("</content>"))){
             ofs_newpage<<"    " + tempdoc.substr(l,r - l + 8)<<endl;
         }
         if((l = tempdoc.find("</doc>"))){
             ofs_newpage<<tempdoc.substr(l,6)<<endl;
             ofs_newoffset<< i.getDocid()<<" "<<ofs_newpage.tellp()<<" "<<tempdoc.size() + 21<<endl;//21 代表空格与换行符所带来的偏移
             _offsetLib[i.getDocid()] = make_pair(ofs_newpage.tellp(),tempdoc.size() + 21);
         }
     }
     
}
//测试
void PageLibPreprocessor::shownewDoc(){
    for(auto& i : _pageLib){
        cout<<i.getDoc()<<endl;
    }
}
//测试用
void PageLibPreprocessor::shownewOffset(){
    for(auto& i : _offsetLib){
        cout<<i.first<<" "<<i.second.first<<" "<<i.second.second<<endl;
    }
}
