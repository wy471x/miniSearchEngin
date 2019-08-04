#include"RedisConfig.h"
#include"RedisTool.h"
#include<stdio.h>
#include<string.h>
#include<sstream>

RedisTool::RedisTool(){
    m_redis = NULL;
    init();
}
RedisTool::~RedisTool(){
    if(m_redis != NULL){
        redisFree(m_redis);
    }
}
void RedisTool::init(){
    struct timeval timeout = {1,500000};
    char ip[255];
    strcpy(ip,m_config.getRedisIP().c_str());
    m_redis = redisConnectWithTimeout(ip,m_config.getRedisPort(),timeout);
    if(m_redis->err){
      printf("RedisTool: Connection error:%s\n",m_redis->errstr);

    }else{
        cout<<"init redis tool success."<<endl;
    }
}
int RedisTool::setString(string key,string value){
    if(m_redis == NULL || m_redis->err)
    {
        cout<<"Redis init Error!"<<endl;
        init();
        return -1;
    }
    redisReply* reply;
    reply = (redisReply*)redisCommand(m_redis,"set %s %s",key.c_str(),value.c_str());
    cout<<"set string type = "<<reply->type<<endl;
    int result = 0;
    if(reply == NULL){
        redisFree(m_redis);
        result = -1;
        cout<<"set string fail:reply->str = NULL"<<endl;
        return -1;
    }
    else if(strcmp(reply->str,"OK") == 0){
        result = 1;
    }else {
        result = -1;
        cout<<"set string fail."<<reply->str<<endl;
    }
    freeReplyObject(reply);
    return result;
}
string RedisTool::getString(string key){
    if(m_redis == NULL|| m_redis->err){
        cout<<"Redis init Error!"<<endl;
        init();
        return string();
    }
    redisReply* reply;
    reply = (redisReply*)redisCommand(m_redis,"get %s",key.c_str());
    if( NULL == reply){
        redisFree(m_redis);
        m_redis = NULL;
        cout<<"ERROR getstring: reply = NULL"<<endl;
        return string();
    }
    else if(reply->len <= 0){
        freeReplyObject(reply);
        return string();
    }
    else{
        stringstream sstr;
        sstr<<reply->str;
        freeReplyObject(reply);
        return sstr.str();
    }
}
