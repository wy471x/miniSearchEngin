#ifndef _H_REDIS_TOOLS_
#define _H_REDIS_TOOLS_
#include "RedisConfig.h"
#include <iostream>
#include <vector>
#include <hiredis/hiredis.h>

using namespace std;

class RedisTool
{
public:
    RedisTool();
    ~RedisTool();
    int setString(string key, string value);
    string getString(string key);
private:
    void init();
    redisContext *m_redis;
    RedisConfig m_config;
};

#endif
