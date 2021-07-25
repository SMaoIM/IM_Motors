// File Name: process_server.c
// Author: jiujue
// Created Time: 2019年03月10日 星期日 20时29分18秒

 #include<stdio.h>
 #include<string.h>
 #include<stdlib.h>
 #include<time.h>
 #include<math.h>
 #include <unistd.h> 
#include <sys/socket.h>
 #include <ctype.h>
 #include <sys/types.h>
#include <arpa/inet.h>
 #include <signal.h>
#include <sys/wait.h>
 #include <errno.h>
 #include <pthread.h>

 typedef struct sock_info
 {
    int m_fd;
    pthread_t m_pthid;
    struct sockaddr_in m_addr;

 }Sock_info;
 
 void* works(void* arg)
 {
    Sock_info *c_info = (Sock_info*)arg;
 
   //obtain client info
    int client_port = ntohs(c_info->m_addr.sin_port);
    char client_Ip[64];

     socklen_t buf_len = sizeof(client_Ip);

     inet_ntop(AF_INET, (void*)&c_info->m_addr.sin_addr.s_addr,client_Ip,buf_len);
  
    printf("\t\t\t\t Ip %s, port %d,connect successful\n",client_Ip,client_port);
 
    while(1)
    {
        char buf[1024] = {0};
        int read_len = read(c_info->m_fd, buf, sizeof(buf));
        if(read_len > 0)
         {
             buf[read_len+1]='\n';
             printf("->-> Obtain if of ,Ip %s, port %d, send info: %s\n",client_Ip,client_port,buf);
            write(c_info->m_fd,buf,strlen(buf));
         }
         else if(0 == read_len)
        {
            printf("\t\t\t\t Ip %s, port %d, disconnect\n",client_Ip,client_port);
            break;
        }
         else if(-1 == read_len)
       {
            perror("read error");
             exit(1);
        }
     }
 
     return NULL;
}

 int main(int argc, const char* argv[])
 {
    if(argc < 3)
    {
        printf("eg: ./app IP Port\n");
        exit(1);
    }
     short port = atoi(argv[2]);

     int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == lfd)
    {
       perror("socket error");
        exit(1);
     }

     struct sockaddr_in serv;
     serv.sin_port = htons(port);
    serv.sin_family = AF_INET;
     inet_pton(AF_INET, argv[1], &serv.sin_addr.s_addr);
 
    int res = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));
    if(-1 == res)
     {
        perror("bind error");
        exit(1);
    }

    res = listen(lfd, 128);
     if(-1 == res)
     {
         perror("listen error");
         exit(1);
     }
 

    while(1)
    {
         printf("\a Wait accepting...\n");
        struct sockaddr_in client_add;
 
        socklen_t len = sizeof(client_add);

       int cfd = accept(lfd,(struct sockaddr*)&client_add, &len); 
         
         while(-1 == cfd && cfd == EINTR)
        {
             cfd = accept(lfd,(struct sockaddr*)&client_add, &len); 
         }
        
        // supply son pthread info
        Sock_info* s_info =(Sock_info*)malloc(sizeof(Sock_info));

        s_info->m_fd = cfd;
        s_info->m_addr = client_add;

        int res = pthread_create(&s_info->m_pthid, NULL, works, (void*)s_info);
        if(res == -1)
         {
            perror("pthread_creat error");
             exit(1);
         }
        pthread_detach(s_info->m_pthid);

     }

    close(lfd);
     return 0 ;
 }