第一部分

该socket 的功能是在客户端输入了两个浮点数组成的字符串，在服务端接受后将其转换为浮点数输出，统一时刻只能有一个客户端连接

服务端：

tcp_server.cpp

**[cpp]** [view plain](https://blog.csdn.net/deyuzhi/article/details/51725074#) [copy](https://blog.csdn.net/deyuzhi/article/details/51725074#)

1. \#include <stdio.h> 
2. \#include <iostream> 
3. \#include <stdlib.h> 
4. \#include <netinet/in.h> 
5. \#include <unistd.h> 
6. \#include <sys/socket.h> 
7. \#include <arpa/inet.h> 
8. \#include <string.h> 
9. **using** **namespace** std; 
10.  
11. \#define PORT 10001 
12.  
13. **int** main() 
14. { 
15.   **struct** sockaddr_in s_in;//server address structure 
16.   **struct** sockaddr_in c_in;//client address structure 
17.   **int** l_fd,c_fd; 
18.   socklen_t len; 
19.   **char** buf[100];//content buff area 
20.   string tem; 
21.   **float** x_value = 0.0; 
22.   **float** y_value = 0.0; 
23.   memset((**void** *)&s_in,0,**sizeof**(s_in)); 
24.  
25.   s_in.sin_family = AF_INET;//IPV4 communication domain 
26.   s_in.sin_addr.s_addr = INADDR_ANY;//accept any address 
27.   s_in.sin_port = htons(PORT);//change port to netchar 
28.  
29.   l_fd = socket(AF_INET,SOCK_STREAM,0);//socket(int domain, int type, int protocol) 
30.   bind(l_fd,(**struct** sockaddr *)&s_in,**sizeof**(s_in)); 
31.   listen(l_fd,1);//同时只能有一个连接 
32.  
33.   cout<<"begin"<<endl; 
34.   **while**(1){ 
35. ​    c_fd = accept(l_fd,(**struct** sockaddr *)&c_in,&len); 
36. ​    **while**(1){ 
37. ​      **for**(**int** j=0;j<100;j++){ 
38. ​        buf[j] = 0; 
39. ​      } 
40. ​      **int** n = read(c_fd,buf,100);//read the message send by client 
41. ​      **if**(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){ 
42. ​        cout << "q pressed\n"; 
43. ​        close(c_fd); 
44. ​        **break**; 
45. ​      } 
46. ​      // inet_ntop(AF_INET,&c_in.sin_addr,addr_p,16);//“二进制整数” －> “点分十进制 
47. ​      tem = buf; 
48. ​      x_value = atof(tem.substr(0,5).c_str()); 
49. ​      y_value = atof(tem.substr(5,5).c_str()); 
50. ​      cout << x_value << "dd" << y_value <<endl; 
51. ​      write(c_fd,buf,n);//sent message back to client 
52. ​    } 
53.   } 
54.   **return** 0; 
55. } 

客户端：

tcp_client.cpp

**[cpp]** [view plain](https://blog.csdn.net/deyuzhi/article/details/51725074#) [copy](https://blog.csdn.net/deyuzhi/article/details/51725074#)

1. \#include <stdio.h> 
2. \#include <string.h> 
3. \#include <iostream> 
4. \#include <stdlib.h> 
5. \#include <string> 
6. \#include <unistd.h> 
7. \#include <arpa/inet.h> 
8. \#include <sys/socket.h> 
9. \#define BUF_SIZE 100 
10. \#define ADDR "127.0.0.1" //在本机测试用这个地址，如果连接其他电脑需要更换IP 
11. \#define SERVERPORT 10001 
12. **using** **namespace** std; 
13. **int** main(**int** argc, **char** *argv[]) 
14. { 
15.   **int** sock; 
16.   **char** opmsg[BUF_SIZE]; 
17.   **char** get_msg[BUF_SIZE] = {0}; 
18.   **int** result, opnd_cnt, i; 
19.   **int** len; 
20.   **bool** if_first = **true**; 
21.   **struct** sockaddr_in serv_addr; 
22.  
23.   sock = socket(PF_INET, SOCK_STREAM, 0); 
24.   **if**(sock == -1){ 
25. ​    **return** -1; 
26.   } 
27.   memset(&serv_addr, 0, **sizeof**(serv_addr)); 
28.   serv_addr.sin_family = AF_INET; 
29.   serv_addr.sin_addr.s_addr = inet_addr(ADDR); // 注释1 
30.   serv_addr.sin_port = htons(SERVERPORT); 
31.   **if**(connect(sock, (**struct** sockaddr*) &serv_addr, **sizeof**(serv_addr))==-1){ // 注释2 
32. ​    cout << "connect error\n"; 
33. ​    **return** -1; 
34.   } 
35.   **else**{ 
36. ​    cout << "connected ...\n" << endl; //注释3 
37.   } 
38.  
39.   **while**(1){ 
40. ​    fgets(opmsg, BUF_SIZE, stdin);     
41. ​    len = write(sock, opmsg, strlen(opmsg)); // 注释4 
42. ​    **if**(!strcmp(opmsg, "q\n") || !strcmp(opmsg, "Q\n")) 
43. ​    { 
44. ​      puts("q pressed\n"); 
45. ​      **break**; 
46. ​    } 
47. ​    **else** 
48. ​    { 
49. ​      **int** read_msg = read(sock, get_msg, len); 
50. ​      cout << "send length:" << len << " get:" << get_msg << endl; 
51. ​    } 
52.   } 
53.   close(sock); 
54.   **return** 0; 
55. } 

两个放在同一个文件夹下，先编译后运行。

编译并运行服务端：

**[cpp]** [view plain](https://blog.csdn.net/deyuzhi/article/details/51725074#) [copy](https://blog.csdn.net/deyuzhi/article/details/51725074#)

1. g++ tcp_server.cpp -o tcp_server.out -lstdc++ 
2.  
3. ./tcp_server.out 





打开另一个窗口编译并运行客户端：

**[cpp]** [view plain](https://blog.csdn.net/deyuzhi/article/details/51725074#) [copy](https://blog.csdn.net/deyuzhi/article/details/51725074#)

1. g++ tcp_client.cpp -o tcp_client.out 
2.  
3. ./tcp_client.out 



在客户端输入字符串例如1.2223.444按回车，服务端会将其转换为浮点数输出，！！！！！！！！注意每个浮点数带上小数点在内必须是总共五位，否则会出错！！！！！！！！！