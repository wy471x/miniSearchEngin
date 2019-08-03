#include"WordQueryServer.h"
//连接回调函数
void WordQueryServer::onConnection(const TcpConnectionPtr& conn){
    cout<<conn->toString()<<" has connected!"<<endl;
  //  Train connectMessage;
  //  memset(&connectMessage,0,sizeof(connectMessage));
  //  strcpy(connectMessage.buff,"Welcome to miniSearchEngin!");
  //  connectMessage.len = strlen(connectMessage.buff);
  //  conn->send(connectMessage);
}
//消息处理函数
void WordQueryServer::onMessage(const TcpConnectionPtr& conn){
    cout<<"onMessage..."<<endl;
    string msg = conn->receive();
    cout<<"receive message from client:"<<endl;
    cout<<msg<<endl;
    _pool.addTask(bind(&WordQueryServer::doTaskThread,this,conn,msg));
}
//关闭回调函数
void WordQueryServer::onClose(const TcpConnectionPtr& conn){
    cout<<"onClose..."<<endl;
    cout<<conn->toString()<<" has closed!"<<endl;
}
//线程执行任务
void WordQueryServer::doTaskThread(const TcpConnectionPtr& conn,const string& msg){
     string resstr = _wordQuery.doQuery(msg);
   //  cout<<resstr<<endl;
     int sz = resstr.size();
     string message(to_string(sz));
     message.append("\n").append(resstr);
     conn->sendInLoop(message);
}
//业务处理函数
void WordQueryServer::start(){
    _pool.start();
    _tcpServer.start();
}
