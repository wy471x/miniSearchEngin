#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<json/json.h>
#include<iostream>
#include<string>
#define MAXSIZE 1000000
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)
using namespace Json;
using namespace std;
void do_service(int sockfd);
int main(int argc, const char *argv[])
{
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if(peerfd == -1)
        ERR_EXIT("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.4.184"); //localhost
    //addr.sin_addr.s_addr = INADDR_ANY; //localhost
    addr.sin_port = htons(8888);
    socklen_t len = sizeof addr;
    if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
        ERR_EXIT("Connect");
    //处理刚连接时的回传信息,如：welcome to ...
  // char buf[1024];
  // memset(buf, 0, sizeof(buf));
  // read(peerfd, buf, sizeof buf);
  // printf("%s\n", buf);
    //处理客户端信息
    do_service(peerfd);
    return 0;
}



void do_service(int sockfd)
{
    char recvbuf[MAXSIZE] = {0};
    char sendbuf[1024] = {0};
    while(1)
    {
        fgets(sendbuf, sizeof sendbuf, stdin);
        write(sockfd, sendbuf, strlen(sendbuf));
		//sleep(5);

        //read
       // int datalen;
       // read(sockfd,&datalen,sizeof(datalen));
      //  string len;
      //  char c;
      //  
      //  while(read(sockfd,&c,1)){
      //      if(c != '\n'){
      //        len.push_back(c);
      //      }else{
      //          break;
      //      }
      //  }
      //  cout<<len<<endl;
      //  int datalen = stoi(len);
        int nread = read(sockfd,recvbuf,sizeof(recvbuf));
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0) //代表链接断开
        {
            printf("server close!\n");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        
        printf("%s",recvbuf);

        memset(recvbuf, 0, sizeof recvbuf);
        memset(sendbuf, 0, sizeof sendbuf);
    }
}




