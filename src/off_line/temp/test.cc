#include <iostream>
#include<fstream>
#include<vector>
#include<regex>
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;
struct RssItem{
 string id;
 string title;
 string link;
 string description;
 string content;
};
class RssReader{
public :
    RssReader(){};
    void parseRss();
    void dump(const char* filename);
private:
 vector<RssItem> _rss;
};
void RssReader::parseRss(){
    XMLDocument doc;
    doc.LoadFile("/home/dunk/miniSearchEngin/data/WEBPAGE/ripepage.xml");
    cout<<"*"<<doc.Error()<<endl;
    if(0 != doc.Error()){
        return;
    }
    XMLElement* item = doc.FirstChildElement("<doc>");
    while(item){
        RssItem tmp;
       XMLElement* item_child = item->FirstChildElement();
       while(item_child){
           if(strcmp(item_child->Name(),"<docid>") == 0){
               tmp.id.append(item_child->GetText());
           }else
           if(strcmp(item_child->Name(),"title") == 0){
               tmp.title.append(item_child->GetText());
           }else
           if(strcmp(item_child->Name(),"link") == 0){
               tmp.link.append(item_child->GetText());
           }else
           if(strcmp(item_child->Name(),"content") == 0){
               tmp.content.append(item_child->GetText());                  
           }
           item_child = item_child->NextSiblingElement();
       }
      _rss.push_back(tmp);
       item = item->NextSiblingElement();
    }
}
void RssReader::dump(const char* filename){
    ofstream ofs;
    ofs.open(filename,ios::app|ios::out);
    vector<RssItem>::iterator begin = _rss.begin();
    vector<RssItem>::iterator end = _rss.end();
    int docid = 1;
    //std::regex label("<[^>]*>");
    regex label("<.*?>");
    for(auto it = begin; it != end;it++){
        ofs<<"<doc>"<<endl;
        ofs<<"  <docid>"<<(*it).id<<"</docid>"<<endl;
        ofs<<"  <title>"<<(*it).title<<"</title>"<<endl;
        ofs<<"  <link>"<<(*it).link<<"</link>"<<endl;
        //cout<<regex_replace((*it).content,label,"")<<endl;
       // istringstream istr(regex_replace((*it).content,label,""));
       // string line;
       // getline(istr,line);
       // cout<<docid<<endl;
       // cout<<line<<endl;
        ofs<<"  <content>"<< regex_replace((*it).content,label,"")<<"</content>"<<endl;
        ofs<<"</doc>"<<endl;
        docid++;
    }
}
int main()
{
    RssReader s;
    s.parseRss();
    s.dump("pagelib.dat");
    return 0;
}

