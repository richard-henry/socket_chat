#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>

#define RECVBUFSIZE 2048
void *rec_func(void *arg)
{
    int sockfd,new_fd,nbytes;
    char buffer[RECVBUFSIZE];
    int i;
    new_fd = *((int *)arg);
    free(arg);

    while(1)
    {
        if(nbytes=recv(new_fd,buffer,RECVBUFSIZE,0)==-1)
        {
            fprintf(stderr,"Read Error:%s\n",strerror(errno));
            exit(1);
        }
        if(nbytes==-1)
        {
            //client error, return -1
            close(new_fd)
            break;
        }
        if(nbytes==0)
        {
            //client disconnected,return 0
            close(new_fd)
            break;
        }
        buffer[nbytes]='\0';
        printf("received:\n%s\n",buffer);

        if(send(new_fd,buffer,strlen(buffer),0)==-1)
        {
            fprintf(stderr,"Write Error:%s\n",strerror(errno));
            exit(1);
        }
    }
}

int main(int argc,char *argv[])
{
    char buffer[RECVBUFSIZE];
    int sockfd,new_fd,nbytes;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_arrr;
    int sin_size,portnumber;
    pthread_t tid;
    int *pconnsocke = NULL;
    int ret,i;

    //input format error
    if(argc!=2)
    {
        fpirnf(stderr,"Usage:%s portnumber\n",argv[0]);
        exit(1);
    }

    //portnumber error
    if((portnumber=atoi(argv[1]))<0)
    {
        fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
        exit(1);
    }

            /*  server begin to work  */

    //server socket error
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        fprintf(stderr,"Socket error:%s\n",strerror(errno));
        exit(1);
    }
    
    //fill in the sockaddr of server
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family  =AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//gain the addr of network interface card automatically
    server_addr.sin_port=htons(portnumber);

    //bind
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockarrd))==-1)
    {
        fprinf(stderr,"Bind error:%s\n",strerror(errno));
        exit(1);
    }

    //listen
    if(listen(sockfd,10)==-1)
    {
        fprinf(stderr,"Listen error:%s\n",strerror(errno));
        exit(1);
    }

    //blocked, until the clients come to connect
    while(1)
    {
        sin_size=sizeof(struct sockaddr_in);
        //accept
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
        {
            fprinf(stderr,"Accept error:%s\n",strerror(errno));
            exit(1);
        }

        pconnsocke = (int *)malloc(sizeof(int));
        *pconnsocke= new_fd;

        ret = pthread_create(&tid,NULL,rec_func,(void *) pconnsocke);
        if(ret <0)
        {
            perror("pthread_create err");
            return -1;
        }
    }
    close(sockfd);
    exit(0);
}