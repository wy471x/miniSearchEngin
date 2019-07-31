#include <iostream>
#include<fstream>
#include<vector>
#include<boost/regex.hpp>
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;
struct RssItem{
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
    doc.LoadFile("coolshell.xml");
    cout<<"*"<<doc.Error()<<endl;
    if(0 != doc.Error()){
        return;
    }
    XMLElement* root = doc.RootElement();
    XMLElement* startelem = root->FirstChildElement("channel");
    if(NULL == startelem){
        cout<<"get first child  element error"<<endl;
        return ;
    }
    XMLElement* nextelem =startelem->FirstChildElement("title");
    while(nextelem){
        if(strcmp(nextelem->Name(),"item") == 0){
            break;
        }
        nextelem = nextelem->NextSiblingElement();
    }
    XMLElement* item = nextelem;
    while(item){
        RssItem tmp;
       XMLElement* item_child = item->FirstChildElement();
       while(item_child){
           if(strcmp(item_child->Name(),"title") == 0){
               tmp.title.append(item_child->GetText());
           }
           if(strcmp(item_child->Name(),"link") == 0){
               tmp.link.append(item_child->GetText());
           }
           if(strcmp(item_child->Name(),"description") == 0){
               tmp.description.append(item_child->GetText());
           }
           if(strcmp(item_child->Name(),"content:encoded") == 0){
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
    ofs.open(filename,ios::app|ios::in);
    vector<RssItem>::iterator begin = _rss.begin();
    vector<RssItem>::iterator end = _rss.end();
    int docid = 1;
    //std::regex label("<[^>]*>");
    boost::regex label("<.*?>");
    for(auto it = begin; it != end;it++){
        ofs<<"<doc>"<<endl;
        ofs<<"  <docid>"<<docid<<"</docid>"<<endl;
        ofs<<"  <title>"<<(*it).title<<"</title>"<<endl;
        ofs<<"  <link>"<<(*it).link<<"</link>"<<endl;
        cout<<boost::regex_replace((*it).content,label,"")<<endl;
        ofs<<"  <content>"<< boost::regex_replace((*it).content,label,"")<<"</content>"<<endl;
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

