#ifndef _TCP_CHAT
#define _TCP_CHAT

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<fcntl.h>

#define SERVER_PORT 8888

//online user information
struct ONLINE{
    int fd;
    int flage;
    char name[32];
};

#define MAX_USER_NUM 100

//the struct of C/S communication
struct protocol{
    int cmd;
    int state;
    char name[32];
    char data[64];
    };

//cmd
#define BROADCAST 0X00000001
#define PRIVATE 0x00000002
#define REGISTE 0X00000004
#define LOGIN 0x00000008
#define ONLINEUSER 0x00000010
#define LOGOUT 0x00000020

//return code
#define OP_OK 0X800000
#define ONLINEUSER_OK 0X80000001
#define ONLINEUSER_OVER 0x80000002
#define NAME_EXIST 0x80000003
#define USER_LOGED 0X80000005
#define USER_NOT_REGIST 0X80000006
#endif