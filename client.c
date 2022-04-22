#include "chat.h"
int sockfd;
int addrlen;
struct sockaddr_in server_addr;
pthread_t pid;

int login_f =-1;

void *func(void *arg)
{
	int len;
    char buf[64]={0};
	
	new_fd = *((int *) arg);
	free(arg);
	
	while(1)
	{
		if(login_f !=1)
        {
            continue;
        }

        len = read(sockfd,buf,sizeof(buf));
        if(len<=0){
            close(sockfd);
            return;
        }
        buf[len]='\0';
        printf("%s\n",buf);
    }
}
void broadcast(int fd){

}
void private(int fd){

}
void list_online_user(sockfd){

}
int registe(int fd){
    struct protocol msg,msgback;

    msg.cmd=REGISTE;
    printf("input you username:\n");
    scanf("%s",msg.name);

    write(sockfd,&msg,sizeof(msg));
    read(sockfd,&msgback,sizeof(msgback));
    if(msgback.state!=OP_OK){
        printf("NAME EXISTED, TRY ANOTHER:");
        getchar();
        getchar();
        return -1;
    }
    else{
        printf("Regist Success.\n");
        getchar();
        getchar();
        return 0;
    }
}

int login(int fd){
    struct protocol msg,msgback;
    msg.cmd=LOGIN;
    printf("input you username:\n");
    scanf("%s",msg.name);

    write(sockfd,&msg,sizeof(msg));
    read(sockfd,&msgback,sizeof(msgback));
    if(msgback.state!=OP_OK){
        printf("NAME EXISTED, TRY ANOTHER:");
        getchar();
        getchar();
        return -1;
    }
    else{
        printf("Login Success.\n");
        getchar();
        getchar();
        login_f=1;
        return OP_OK;
    }
}

int logout(int fd){
    close(fd);
    login_f=-1;
}

int main(int argc,char **argv[]){
    int sel;
    int portnumber;
    int min_sel,max_sel;
    int ret;
    struct protocol msg;

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
    server_addr.sin_family  =PF_INET;
    server_addr.sin_port=htons(portnumber);
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);

    addrlen = sizeof(struct sockaddr_in);

    //connect
    if(connect(sockfd,(struct sockaddr *)(&server_addr),addrlen)==-1)
    {
        fprintf(stderr,"Connect Error:%s\n",strerror(errno));
        exit(1);
    }


    //create thread
        pthread_create(&pid,NULL,func,NULL);
    	while(1)
        {
            system("clear");
            if(login_f==-1){
                printf("\t 1 register\n");
                printf("\t 2 login\n");
            }
            else if(login_f==1){
                printf("\t broadcast\n");
                printf("\t private\n");
                printf("\t online_list\n");
            }
            printf("\t quit 0\n");

            fflush(stdin);
            scanf("%d",&sel);
            if(!sel)break;
            if(login_f==1){
                min_sel=3;
                max_sel=5;
            }else if(login_f== -1){
                min_sel=1;
                max_sel=2;
            }

            if(sel<min_sel||sel>max_sel){
                printf("Valid choice, try again.\n");
                continue;
            }
            switch(sel)
            {
                case 1:
                    registe(sockfd);
                    break;
                case 2:
                    ret=login(sockfd);
                    break;
                case 3:
                    broadcast(sockfd);
                    break;
                case 4:
                    private(sockfd);
                    break;
                case 5:
                    list_online_user(sockfd);
                    break;
                case 0:
                    logout(sockfd);
                    break;
                default:
                    break;
            }
            if(!sel)exit(0);
        }
}