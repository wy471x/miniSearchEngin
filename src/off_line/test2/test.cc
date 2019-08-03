#include"PageLibPreprocessor.h"
#include <iostream>
using namespace std;
void test(){
    Configuration conf("/home/dunk/miniSearchEngin/conf/conf");
    PageLibPreprocessor pageprocessor(conf);
    pageprocessor.doProcess();
    //pageprocessor.shownewDoc();
    //pageprocessor.shownewOffset();
}
int main()
{
    test();
    return 0;
}

