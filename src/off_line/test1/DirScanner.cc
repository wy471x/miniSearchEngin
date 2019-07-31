#include"DirScanner.h"
DirScanner::DirScanner(Configuration& conf):_conf(conf){
}
vector<string> DirScanner::files(){
    return _vecFilesfiles;
}
void DirScanner::operator()(const string& dirname){
   
    this->traverse(_conf.getConfigMap()[dirname]);
}
void DirScanner::traverse(const string& dirname){
     DIR* dir;
     struct dirent*  ptr;
     if((dir = opendir(dirname.c_str())) == NULL){
        perror("Open dir error!");
        exit(1);
     }
     while((ptr = readdir(dir))!= NULL){
         if(strcmp(ptr->d_name,".") == 0|| strcmp(ptr->d_name,"..") == 0){
                continue;
         }else if(ptr->d_type == 8){
             string str = dirname + "/" + ptr->d_name;
               _vecFilesfiles.push_back(str);
         }else{
             continue;
         }
     }
     closedir(dir);
}
