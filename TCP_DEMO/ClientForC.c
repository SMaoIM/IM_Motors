// File Name: socket_client.c
// Author: jiujue
 // Created Time: 2019年03月09日 星期六 13时00分05秒
 
 #include<stdio.h>
 #include<string.h>
 #include<stdlib.h>
 #include<time.h>
 #include<math.h>
 #include <unistd.h>
 #include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, const char* argv[])
{
    if(argc < 3)
   {
        printf("eg: ./app Ip Port\n");
        exit(1);
    }
    int port = atoi(argv[2]);

  int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
     {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in serv;
 
    serv.sin_port =  htons(port);
    serv.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serv.sin_addr.s_addr);

   socklen_t len = sizeof(serv);
     int res = connect(fd,(struct sockaddr *) &serv, len);
     if(-1 == res)
    {
        perror("connect error");
       exit(1);
    }
    printf("Connectt server successful!!\n");

    while(1)
    {
         printf("Please input string\n>>");
        char buf[1024]; 
         fgets(buf,sizeof(buf),stdin);
        write(fd, buf, strlen(buf));
        printf("send buf: %s\n",buf);
        len = read(fd,buf,(buf));
         if(len > 0)
        {
          printf("Recv buf: %s\n",buf);
        }else if(len == 0)
        {
            printf("Serer disconnect \n");
           break;
        }
        else if(-1 == len)
       {
           perror("Read errror");
            exit(1);
        }else
       {
           printf("I no now\n");
        }
    }
 
    close(fd);
     return 0 ;
 }