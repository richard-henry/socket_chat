#include "chat.h"
int sockfd;
int addrlen;
struct sockaddr_in server_addr;
pthread_t pid;

int login_f =-1;

void *func(void *arg)
{
	int len;
    char buf[128]={0};
	struct protocol *msg;

	while(1)
	{
		if(login_f !=1)
        {
            continue;
        }
        memset(buf,0,sizeof(buf));
        len = read(sockfd,buf,sizeof(buf));
        if(len<=0){
            close(sockfd);
            return;
        }

        msg= (struct protocol *)buf;
        if((msg->state == ONLINEUSER_OK)&&(msg->cmd == ONLINEUSER))
        {
            printf("%s\t",msg->name);
            continue;
        }
        if((msg->state == ONLINEUSER_OVER)&&(msg->cmd == ONLINEUSER))
        {
            printf("\n");
            continue;
        }
        buf[len]='\0';
        printf("%s\n",buf);
    }
}
void broadcast(int fd){
        struct protocol msg;
        msg.cmd = BROADCAST;
        printf("your message:\n#");
        getchar();
        fgets(msg.data,64,stdin);

        write(fd,&msg,sizeof(msg));
        printf("press following section to continue:\n");
        return;
}
void private(int fd){
        struct protocol msg;
        msg.cmd= PRIVATE;

        printf("input name you want to talk:\n#");
        scanf("%s",msg.name);

        printf("your message:\n#");
        getchar();
        fgets(msg.data,64,stdin);

        write(fd,&msg,sizeof(msg));
        printf("press following section to continue:\n");
        return;
        
}
void list_online_user(sockfd){

    struct protocol msg;
    msg.cmd = ONLINEUSER;

    write(sockfd,&msg,sizeof(msg));
    while(1){
    read(sockfd,&msg,sizeof(msg));
    if(msg.state==ONLINEUSER_OK){
	printf("%s\n", msg.name);
}
else break;
}
return;
}

int registe(int fd){
    struct protocol msg,msgback;
    char buf[128]; 
    msg.cmd=REGISTE;
    printf("input you username:\n");
    scanf("%s",msg.name);

    write(sockfd,&msg,sizeof(msg));
    read(sockfd,&msgback,sizeof(msgback));
    if(msgback.state!=OP_OK){
        printf("NAME EXISTED, TRY ANOTHER.\n");
        printf("press following section to continue:\n");
        getchar();
        return -1;
    }
    else{
        printf("Regist Success.\n");
        printf("press following section to continue:\n");
        //getchar();
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
        printf("NAME ERROR, TRY ANOTHER:\n");
        printf("press following section to continue:\n");
        //getchar();
        return -1;
    }
    else{
        printf("Login Success.\n");
        printf("press following section to continue:\n");
        //getchar();
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
        printf("cmd: %s ip portnumber\n",argv[0]);
        return;
    }
    //get the portnumber
    if((portnumber=atoi(argv[2]))<0)
    {
        fprintf(stderr,"Usage:%s hostname portnumber\n",argv[0]);
        exit(1);
    }
    //create a socket
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1)
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
            //system("clear");
            if(login_f==-1){
                printf("\t 1 register\n");
                printf("\t 2 login\n");
            }
            else if(login_f==1){
                printf("\t 3 broadcast\n");
                printf("\t 4 private\n");
                printf("\t 5 show online_list\n");
            }
            printf("\t 0 quit\n");

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
