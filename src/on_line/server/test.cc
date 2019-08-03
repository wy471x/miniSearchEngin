#include"WordQueryServer.h"
void test(){
    WordQueryServer wordqueryserver("/home/dunk/miniSearchEngin/conf/conf");
    wordqueryserver.start();
}
int main()
{
    test();
    return 0;
}

