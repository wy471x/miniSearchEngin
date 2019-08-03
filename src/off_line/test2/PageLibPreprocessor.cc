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
     while(getline(ifs_page,line)){
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
     double TF;//某词在文档中出现的次数
     double IDF;//逆文档率,权重系数
     double N = _pageLib.size();
     double tempweigh = 0;
     unordered_map<int,vector<pair<string,double>>> ivsd;//对应文档中所有词语的权重系数
     //获取每一个文档中的所有单词或汉字的权重系数
     ofstream ofs("invert");
     int docnum = 1;
     for(auto& i : _pageLib){
         for(auto& j : i.getWordsMap()){
             double DF = 0;//包含该词语的文档数量
             TF = j.second;
             for(auto& u : _pageLib){
                 if(u.getWordsMap().find(j.first)!= u.getWordsMap().end()){
                     DF++; 
                 }
             }
             IDF =log2(N/(DF + 1));
             ofs<<"word :"<<j.first<<" IDF ="<<IDF<<" DF ="<<DF<<" TF ="<<TF<<" N ="<<N<<" ";
             tempweigh = TF * IDF;
             ofs<<tempweigh<<endl;
             ivsd[docnum].push_back(make_pair(j.first,tempweigh));
         }
         ++docnum;
     }
     double weighsum = 0,uw = 0;
     //对权重系数进行归一化处理，并建立倒排索引
    unordered_map<int,vector<pair<string,double>>> tempinvert;
    for(auto& i : ivsd){
        //每个单词的权重之和
        for(auto& u : i.second){
            weighsum +=u.second * u.second;
        }
        //分别计算同一文档下每个单词的归一化的权重值
        for(auto& j : i.second){
           uw = j.second/sqrt(weighsum);
           tempinvert[i.first].push_back(make_pair(j.first,uw));
        }
    }
  //建立倒排索引
    for(auto& i : tempinvert){
        for(auto& j : i.second){
            _invertIndexTable[j.first].push_back(make_pair(i.first,j.second));
        }
    }
}
//存储处理之后的网页库与偏移库及倒排索引表
void PageLibPreprocessor::storeOnDisk(){
     ofstream ofs_newpage(_conf.getConfigMap()["newripepage.lib"]);
     ofstream ofs_newoffset(_conf.getConfigMap()["newoffset.lib"]);
     ofstream ofs_inertindex(_conf.getConfigMap()["invertindextable"]);
     int docnum = 1;
     for(auto& i : _pageLib){
         string tempdoc = i.getDoc();
         size_t l = 0, r = 0;
         string newdoc ="<doc>\n";   
         newdoc +="     "+string("<docid>") +to_string(docnum) +"<docid>\n";
         if((l = tempdoc.find("<title>"))&&(r = tempdoc.find("</title>"))){
             newdoc += "     " + tempdoc.substr(l,r- l + 8) + "\n";
         }
         if((l = tempdoc.find("<link>"))&&( r = tempdoc.find("</link>"))){
             newdoc +="     " + tempdoc.substr(l,r - l + 7) + "\n";
         }
         if((l = tempdoc.find("<content>"))&&( r = tempdoc.find("</content>"))){
             newdoc +="     " + tempdoc.substr(l,r - l + 10) + "\n";
         }
         newdoc +="</doc>\n";
         ofs_newpage<<newdoc;
         ofs_newoffset<<docnum<<" "<<ofs_newpage.tellp()<<" "<<newdoc.size() <<endl;//21 代表空格与换行符所带来的偏移
         _offsetLib[docnum] = make_pair(ofs_newpage.tellp(),newdoc.size());
        ++docnum;
     }
     buildInvertIndexTable();
     for(auto& i : _invertIndexTable){
         ofs_inertindex<<i.first<<" ";
         for(auto& j : i.second){
             ofs_inertindex<<j.first<<" "<<j.second<<" ";
         }
         ofs_inertindex<<endl;
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
