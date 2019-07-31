#include"WordSegmentation.h"
vector<string> WordSegmentation::cut(const string& sentence){
    vector<string> words;
    jieba.Cut(sentence,words,true);
    return words;
}
