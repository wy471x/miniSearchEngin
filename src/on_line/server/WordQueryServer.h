#pragma once
#include"Configuration.h"
#include"WordQuery.h"
#include"TcpServer.h"
#include"Threadpool.h"
#include<iostream>
#include<string>
using namespace std;
using namespace wd;
class WordQueryServer
{
public:
    WordQueryServer(const string& filename)
    :_conf(filename)
     ,_wordQuery(_conf)
     ,_tcpServer(_conf.getConfigMap()["ip"],stoi(_conf.getConfigMap()["port"]))
    ,_pool(stoi(_conf.getConfigMap()["threadnum"]),stoi(_conf.getConfigMap()["queuesize"]))
    {
       _tcpServer.setConnectionCallback(bind(&WordQueryServer::onConnection,this,placeholders::_1));
       _tcpServer.setMessageCallback(bind(&WordQueryServer::onMessage,this,placeholders::_1));
       _tcpServer.setCloseCallback(bind(&WordQueryServer::onClose,this,placeholders::_1));
    
    }
    void start();
    ~WordQueryServer() {}

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn);
    void onClose(const TcpConnectionPtr& conn);
    void doTaskThread(const TcpConnectionPtr& conn,const string& msg);
    Configuration _conf;
    WordQuery _wordQuery;
    TcpServer _tcpServer;
    Threadpool _pool;
};

