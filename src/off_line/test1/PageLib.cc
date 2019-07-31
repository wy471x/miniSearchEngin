#include"PageLib.h"
void PageLib::create(){
     _dirscanner("xml");
    // for(auto& i :_dirscanner.files()){
    //     cout<<i<<endl;
    // }
     int docid = 1;
     //对所有xml文件改成标准格式
     for(auto& i : _dirscanner.files()){
        //cout<<i<<endl;
         XMLDocument doc;
        doc.LoadFile(i.c_str());
        if(0 != doc.Error()){
           cout<<"parse xml file error"<<endl;    
        }
        XMLElement* item = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
        regex label("<.*?>");
        while(item){
           string fmtstr;
           XMLElement*  childtitle = item->FirstChildElement("title");
           XMLElement* childlink = item->FirstChildElement("link");      
           XMLElement* childdescription = item->FirstChildElement("description");
           XMLElement* childcontent = item->FirstChildElement("content:encoded");
           string title,link,description,content;
           if(childtitle&&childtitle->GetText()){ 
                title.append(childtitle->GetText());
           }
           if(childlink&&childlink->GetText()){
           link.append(childlink->GetText());
           }
           if(childdescription&&childdescription->GetText()){
                 description.append(childdescription->GetText());
           }
           if(childcontent&&childcontent->GetText()){
                 content.append(regex_replace(childcontent->GetText(),label,""));
           }else{
               content.append(regex_replace(childdescription->GetText(),label,""));
           }
           if(title.empty()&&!content.empty()){
               istringstream istr(content);
               getline(istr,title);
           } 
           if(!title.empty()&&!link.empty()){ 
            fmtstr  = "<doc>\n";
            fmtstr += "     <docid>" + to_string(docid) + "</docid>\n";
            fmtstr += "     <title>" + title  + "</title>\n";
            fmtstr += "     <link>"  + link   + "</link>\n";
            fmtstr += "     <content>" + content + "</content>\n";
            fmtstr += "</doc>\n";
            _vecfiles.push_back(fmtstr);
            ++docid;
           }
            item = item->NextSiblingElement("item");
       } 
     }
}
//存储网页库与网页偏移库
void PageLib::store(){
     ofstream ofs_page(_conf.getConfigMap()["ripepage.lib"]);
     ofstream ofs_offset(_conf.getConfigMap()["offset.lib"]);
     int docid = 1;
     for(auto& i: _vecfiles){
         ofs_page<<i;
         _offsetlib.insert(make_pair(docid,make_pair(ofs_page.tellp(),i.size())));
         ++docid;
     }
     for(auto& i : _offsetlib){
         ofs_offset<<i.first<<" ";
         ofs_offset<<i.second.first<<" "<<i.second.second<<endl;
     }
}
void PageLib::showpagelib(){
    for(auto& i : _vecfiles){
        cout<<i;
    }
}
