#include"WordQuery.h"
struct SimilarityCompare{
    bool operator()(const pair<int,double>& lhs,
                    const pair<int,double>& rhs)
    {
        return lhs.second  > rhs.second;
    }
};
string WordQuery::doQuery(const string& str){
    loadLibrary();
    vector<string> querywords;
    if(str.size() > 0){
       querywords = _jieba.cut(str);
    }
   // cout<<"after cut result:";
   // for(auto i : querywords){
   //     cout<<i<<" ";
   // }
   // cout<<endl;
    for(auto i : querywords){
        if(_invertIndexTable.find(i) == _invertIndexTable.end()){
            cout<<"\""<<i<<"\":"<<" can not find."<<endl;
            return returnNoAnswer();
        }
    }
    vector<pair<int,vector<double>>> resultvec;
    if(executeQuery(querywords,resultvec)){
        vector<double> queryweight = getQueryWordsWeightVector(querywords);         
    //    cout<<"queryweight :"<<endl;
    //    for(auto i : queryweight){
    //        cout<<i<<" ";
    //    }
    //    cout<<endl;
        vector<pair<int,double>> cosnum;
        double qwabs = 0;
        for(auto i : queryweight){
            qwabs += pow(i,2);
        }
        qwabs = sqrt(qwabs);
        //每个网页的模
        vector<pair<int,double>> resultabs;
        for(auto i : resultvec){
            double sum = 0;
            for(auto j : i.second){
                sum += pow(j,2);
            }
            resultabs.push_back(make_pair(i.first,sqrt(sum)));
        }
        //查看对应的查询结果的模
   //     cout<<"model :"<<endl;
   //     for(auto i : resultabs){
   //         cout<<i.first<<" "<<i.second<<endl;
   //     }
        
        //每个网页的与查询词的内积
        vector<pair<int,double>> resultmul;
        for(auto i : resultvec){
            double mul = 0;
            for(size_t j = 0; j < i.second.size(); ++j){
                  mul +=  i.second[j] * queryweight[j];        
            }
            resultmul.push_back(make_pair(i.first,mul));
        }
        //查看每个网页与查询词内积
        
    //    cout<<" inner :"<<endl;
    //    for(auto i : resultmul){
    //        cout<<i.first<<" "<<i.second<<endl;
    //    }
        
        for(auto i : resultvec){
            cosnum.push_back(make_pair(i.first,resultmul[i.first].second /resultabs[i.first].second ));
        }
        stable_sort(cosnum.begin(),cosnum.end(),SimilarityCompare());
        vector<int> resdocid;
        for(auto i : cosnum){
//            cout<<i.first<<" "<<i.second<<endl;
            resdocid.push_back(i.first);
        }
//        cout<<"-------------------"<<endl;
//        for(auto i : resdocid){
//            cout<<i<<endl;
//        } 
        
        
        return createJson(resdocid,querywords);  
    }else{
        return returnNoAnswer();
    }
}
//加载库文件
void WordQuery::loadLibrary(){
  ifstream ifs_newpage(_conf.getConfigMap()["newripepage.lib"]);
  ifstream ifs_newoffset(_conf.getConfigMap()["newoffset.lib"]);
  ifstream ifs_invertindextable(_conf.getConfigMap()["invertindextable"]);
  //以下部分是加载新文档库
  string line;
  string alldoc;
  while(getline(ifs_newpage,line)){
      alldoc += line;
  }
  //新文档库
  size_t i = 0,j;
  string doc;
  //用于测试用的文件
  ofstream ofs("newpagelib");
  while((j = alldoc.find("</doc>",i)) < alldoc.size()){
      int size = j + 6 - i;//每个文档的大小
      doc = alldoc.substr(i,size);
      
      ofs<<doc<<endl;
      
      WebPage webpage(doc,_conf,_jieba);
      _pagelib.push_back(webpage);
      i = j + 6;
  }
  //以下为加载网页偏移库
  ofstream ofs1("newoffsetlib");
  int id,offset,len;
  while(ifs_newoffset>>id>>offset>>len){
      ofs<<id<<" "<<offset<<" "<<len<<endl;
      _offsetlib[id] = make_pair(offset,len);
  }
  ofstream ofs2("invertindex");
  //以下加载倒排索引表
  string line1;
  while(getline(ifs_invertindextable,line1)){
      istringstream istr(line1);
      string word;
      istr>>word;
      int id;
      double weight;
      while(istr>>id>>weight){
          _invertIndexTable[word].insert(make_pair(id,weight));
      }
  }
 ifs_newpage.close();
 ifs_newoffset.close();
 ifs_invertindextable.close();
}
//计算查询词的权重
vector<double> WordQuery::getQueryWordsWeightVector(const vector<string>& queryWords){
  //统计词频
  map<string,int> wordFreq;
  for(auto item : queryWords){
      ++wordFreq[item];
  }
  //计算权重
  vector<double> weightList;
  double TF,DF,IDF;
  double weightsum = 0; 
  double N = _pagelib.size();//文章总数
 // cout<<"N :"<<N<<endl;
  for(auto& i : wordFreq){
      DF = _invertIndexTable[i.first].size();//某个词在所有文章中出现的次数
//      cout<<"DF :"<<DF<<endl;
      TF = i.second; //某词在文章中出现的次数
//      cout<<"TF :"<<TF<<endl;
      IDF = log2(N /(DF + 1));
//      cout<<"IDF :"<<IDF<<endl;
      weightsum += pow(TF*IDF,2);
      weightList.push_back(TF * IDF);
  }
//  cout<<"weightsum : "<<weightsum<<endl;
  //测试用，输出归一化前的权重值
//  cout<<"weight : "<<endl;
//  for(auto i : weightList){
//      cout<<i<<" ";
//  }
//  cout<<endl;
  //归一化处理
  for(auto& i : weightList){
       i = i / sqrt(weightsum);
  }
//  cout<<endl;

  //测试，输出所有权重值
//  for(auto i : weightList){
//      cout<<i<<" ";
//  }
//  cout<<endl;
  return weightList;
}
//执行查询
bool WordQuery::executeQuery(const vector<string>& querywords,vector<pair<int,vector<double>>>& resultVec){
    if(querywords.size() == 0){
        cout<<" empty string not find."<<endl;
        return false;
    }
  //  vector<double> qw = getQueryWordsWeightVector(querywords);
    vector<set<pair<int,double>>> dw;
    for(auto i : querywords){
        if(_invertIndexTable.find(i) != _invertIndexTable.end()){
               dw.push_back(_invertIndexTable.find(i)->second);
        }
    }
    map<int,vector<double>> mivd;
    for(auto i : dw){
        for(auto j : i){
            mivd[j.first].push_back(j.second);
        }
    }
    //测试用，查看找到的集合
#if 0    
    for(auto i : dw){
        for(auto j : i){
           cout<<j.first<<" "<<j.second<<endl;
        }
        cout<<endl;
    }
#endif
    //删除对应的文档中权重值集合小于3的
    for(auto i = mivd.begin(); i != mivd.end(); ++i){
        if((*i).second.size() < querywords.size()){
            mivd.erase(i);
        }
    }
    //当交集不为空时
    if(mivd.size() > 0){
        for(auto i : mivd){
        //       cout<<i.first<<" ";
         //   for(auto j : i.second){
        //        cout<<j<<" ";
        //    }
         //   cout<<endl;
            resultVec.push_back(i);
        } 
       return true;
    }
    //交集为空
    return false;
}
//生成json字符串
string WordQuery::createJson(vector<int>& docidvec,const vector<string>& querywords){
   Value root;
   Value arr;
   int cnt = 0;
  // cout<<"createJson func :"<<endl;
   for(auto id : docidvec){
     // cout<<id<<endl;
      string summary = _pagelib[id - 1].summary(querywords);
   //   cout<<summary<<endl;
      string title = _pagelib[id - 1].getTitle();
      string url = _pagelib[id - 1].getUrl();
      Value elem;
      elem["title"] = title;
      elem["summary"] = summary;
      elem["url"] = url;
      arr.append(elem);
      if(++cnt == 10){
          break;
      }
   }
   root["files"] = arr;
   StyledWriter writer;
   return writer.write(root);
}
//无结果是返回值
string WordQuery::returnNoAnswer(){
    Value root;
    Value arr;
    Value elem;
    elem["title"] = "404,Not Found Error";
    elem["summary"] = "Sorry,no result for your want";
    elem["url"] = "---------No Link---------";
    arr.append(elem);
    root["files"] = arr;
    StyledWriter writer;
    return writer.write(root);
}
