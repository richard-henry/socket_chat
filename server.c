#include "chat.h"

struct ONLINE online[MAX_USER_NUM];

void del_user_online(int index){
    int i;
    char buf[128]={0};

    if(index<0)return;
    online[index].fd = -1;
    sprintf(buf,"%s offline.\n",online[index].name);
    //remind other users someone is offline.
    for(int i=0;i<MAX_USER_NUM;i++){
        if(online[i]_fd ==-1)continue;
        write(online[i].fd,buf,strlen(buf));
    }
    return;
}

int add_user(itn sockfd,struct protocol *msg){
    int i,index=-1;
    char buf[128]={0};

    for(i=0;i<64;i++)
    {
        if(online[i].flage ==-1){
            online[i].flage=1;
            strcpy(online[i].name,msg->name);
            strcpy(online[i].name,msg->name);
            printf("regist %s to %d. \n",msg->name,i);
            index=i;
        }
    }
    return index;
}
void broadcast(int index,struct protocol *msg){

}
int find_dest_user_online(int sockfd,int *index struct protocol *msg){
    int i;
    for(i=0;i<MAX_USER_NUM;i++){
        if(online[i].flage==-1)continue;
        if(strcmp(msg->name,online[i].name)==0){
            if(online[i].fd==-1)
            {
                online[i].fd=sockfd;
                *index=i;
                return OP_OK;
            }
            else{
                printf("%s had logined.\n",online[i].name);
                return USER_LOGED;
            }
        }
    }

}

int find_dest_user(char *name){
    int i;
    for(i=0;i<MAX_USER_NUM;i++)
    {
        if(online[i].flage==-1)continue;
        if(strcmp(name,online[i].name)==0){
            return i;
        }
    }
    return -1;
}

void private(int index, struct protocol *msg){

}

void list_online_user(int index){

}

void registe(int sockfd,int *index,struct protocol *msg){
    int dest_index;
    char buf[128];
    struct protocol msg_back;

    msg_back.cmd=REGISTE;

    //find
    dest_index = find_dest_user(msg->name);

    if(dest_index ==-1){
        //register
        *index=add_user(sockfd,msg);
        online[*index].flage=1;
        msg_back.state= OP_OK;

        printf("User %s regist success!\n",msg->name);
        write(sockfd,&msg_back,sizeof(msg_back));
        return;
    }else{
        msg_back.state= NAME_EXIST;
        printf("user %s exist!\n",msg->name);
        write(sockfd,&msg_back,sizeof(msg_back));
        return;
    }
}

void login(int sockfd,int *index, struct protocol *msg){
    int i;
    int ret;
    char buf[128];
    struct protocol msg_back;

    msg_back.cmd=LOGIN;

    //find
    ret = find_dest_user_online(sockfd,index,msg);

    if(ret !=OP_OK){
        //user not found.
        msg_back.state=ret;
        strcpy(buf,"there is no this user.\n");
        printf("user %s login failed.\n",msg->name);

        write(sockfd,&msg_back,sizeof(msg_back));
        return;
    }else{
        msg_back.state= OP_OK;
        printf("user %s login succeed! index= %d \n",msg->name,*index);
        write(online[*index].fd,&msg_back,sizeof(msg_back));
        return;
    }
    spritnf(buf,"%s online.\n",online[*index].name);

    for(i=0;i<MAX_USER_NUM;i++){
        if(online[i].fd!=-1)write(online[i].fd,buf,strlen(buf));
    }
}
void *func(void *arg)
{
    int sockfd = *((int *)arg);
    char buf[64];
    int len;
    int index = -1;
    struct protocol msg;

    free(arg);
    //begin to chat
    while(1)
    {
        len = read(sockfd,&msg,sizeof(msg));
        if(len<=0)
        {
            printf("%s offline.\n",online[index].name);

            del_user_online(index);
            close(sockfd);
            return;
        }

        switch(msg.cmd)
        {
            case REGISTE:
            registe(sockfd,&index,&msg);
            break;
            case LOGIN:
            login(sockfd,&index,&msg);
            break;
            case BROADCAST:
            broadcast(index,&msg);
            break;
            case PRIVATE:
            private(index,&msg);
            break;
            case ONLINEUSER:
            list_online_user(index);
            break;
            default:
            break;
        }
    }
}

	
	
int main(int argc,char *argv[])
{
    int lsfd,newfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in cli_addr;
    int addrLen,cliaddrlen;
    int portnumber;
    pthread_t tid;
    int *arg;
    int i;
    char buf[64]={0};
    //input format error
    if(argc!=2)
    {
        fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
        exit(1);
    }

    //portnumber error
    if((portnumber=atoi(argv[1]))<0)
    {
        fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
        exit(1);
    }

            /*  server begin to work  */

    lsfd = socket(PF_INET,SOCK_STREAM,0);
    if(lsfd<0){
        perror("socket fail\n");
        return;
    }

    //fill in the sockaddr of server
    bzero(&my_addr,sizeof(struct sockaddr_in));
    my_addr.sin_family  =AF_INET;
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);//gain the addr of network interface card automatically
    my_addr.sin_port=htons(portnumber);
    addrLen = sizeof(struct sockaddr_in);
    //bind
    if(bind(sockfd,(struct sockaddr *)(&my_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Bind error:%s\n",strerror(errno));
        exit(1);
    }

    //listen
    if(listen(sockfd,5)==-1)
    {
        fprintf(stderr,"Listen error:%s\n",strerror(errno));
        exit(1);
    }
    cliaddrlen = sizeof(struct sockaddr_in);

    for(i=0;i<64;i++){
        online[i].fd= -1;
        online[i].flage=-1;
    }

    //blocked, until the clients come to connect
    while(1)
    {
        //accept
        new_fd=accept(lsfd,(struct sockaddr *)&cli_addr,&cliaddrlen);
        printf("client:ip:%s    port:%d \n",inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);

        arg = (int *)malloc(sizeof(int));
        *arg = newfd;

        pthread_create(&tid,NULL,func,(void *) arg);
        
    }
    close(newfd);
    close(lsfd);
}
