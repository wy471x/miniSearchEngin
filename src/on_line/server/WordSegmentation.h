#pragma once
#include "../../../include/cppjieba/Jieba.hpp"
#include<string>
#include<vector>
using namespace std; 

const char* const DICT_PATH = "../../../data/CPPJIEBA/jieba.dict.utf8";
const char* const HMM_PATH = "../../../data/CPPJIEBA/hmm_model.utf8";
const char* const USER_DICT_PATH = "../../../data/CPPJIEBA/user.dict.utf8";
const char* const IDF_PATH = "../../../data/CPPJIEBA/idf.utf8";
const char* const STOP_PATH = "../../../data/CPPJIEBA/stop_words.utf8";

class WordSegmentation
{
public:
    WordSegmentation():jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_PATH) {} ;
    ~WordSegmentation(){}
    vector<string> cut(const string &sentence);
private:
   cppjieba::Jieba jieba;
};
