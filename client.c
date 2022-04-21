#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#define RECVBUFSIZE 1024

void *func(void *arg)
{
	int sockfd,new_fd,nbytes;
	char buffer[RECVBUFSIZE];
	
	new_fd = *((int *) arg);
	free(arg);
	
	while(1)
	{
		if((nbytes=recv(new_fd,buffer, RECVBUFSIZE,0))==-1)
		{
			fprintf(stderr,"Read Error:%s\n",strerror(errno));
			exit(1);
		}
		buffer[nbytes]='\0';
		printf("I have received:%s\n",buffer);	
	}

}

int main(int argc,char *argv[]){
    int sockfd;
    char buffer[RECVBUFSIZE];
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portnumber,nbytes;
    pthread_t tid;
    int *pconnsocke = NULL;
    int ret;

    //check the number of arguments
    if(argc!=3)
    {
        fprintf(stderr,"Usage:%s hostname portnumber\n",argv[0]);
        exit(1);
    }
    //get the portnumber
    if((portnumber=atoi(argv[2]))<0)
    {
        fprintf(stderr,"Usage:%s hostname portnumber\n",argv[0]);
        exit(1);
    }
    //create a socket
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        fprintf(stderr,"Socket Error:%s\n",strerror(errno));
        exit(1);
    }
    //fill in the struct
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family  =AF_INET;
    server_addr.sin_port=htons(portnumber);
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);

    //connect
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Connect Error:%s\n",strerror(errno));
        exit(1);
    }


    //create thread
    	 pconnsocke = (int *)malloc(sizeof(int));
        *pconnsocke= sockfd;

        ret = pthread_create(&tid,NULL,func,(void *) pconnsocke);
        if(ret <0)
        {
            perror("pthread_create err");
            return -1;
        }
        while(1)
        {
            #if 1
            //send
            printf("input msg:");
            scanf("%s",buffer);
            if(send(sockfd,buffer,strlen(buffer),0)==-1)
            {
                fprintf(stderr,"Send Error:%s\n",strerror(errno));
                exit(1);
            }
            #endif
        }
        close(sockfd);
        exit(0);
}
