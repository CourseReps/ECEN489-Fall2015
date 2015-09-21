#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream>

using namespace std;

#define MYPORT 3490 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 10000000


void sigchld_handler(int s)
{
while(wait(NULL) > 0);
}


int main(void)
{
    int sock_fd, new_fd;
    sockaddr_in myaddr, clientaddr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1, bytes_received, bytes_sent,len;
    char *cmd = new char[100], *chptr = new char;


    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);

    }


    if (setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }


    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(MYPORT);
    myaddr.sin_addr.s_addr = inet_addr("10.202.112.218");
    memset(&(myaddr.sin_zero), '\0', 8);

    cout<<"IP Address of the Server is: "<<"10.202.112.218"<<endl;

    if(bind(sock_fd, (sockaddr *)&myaddr, sizeof(sockaddr)) == -1)
    {
        perror("bind");
        exit(1);

    }

    if((listen(sock_fd, BACKLOG)) == -1)
    {
        perror("listen");
        exit(1);

    }


    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }



        sin_size = (socklen_t)sizeof(sockaddr_in);
        if((new_fd = accept(sock_fd, (sockaddr *)&clientaddr, &sin_size)) == -1)
        {
            perror("accept");
            exit(1);

        }

        cout<<new_fd<<endl;
        cout<<"Server: Connected to: "<< inet_ntoa(clientaddr.sin_addr)<<endl;


        if((bytes_received = recv(new_fd, cmd, MAXDATASIZE, 0)) == -1)
           {
               perror("recv");
               exit(1);
           }

   //     cmd[bytes_received] = '\0';
        cout<< "Command Received: "<<cmd<<endl;

        if(strcmp(cmd, "get_photo") == 0)
        {
            ifstream fin ("/home/karticbhargav/server_image.jpeg");
            char ch;


            while(!fin.eof())
            {
                fin.get(ch);
                chptr = &ch;
                if((bytes_sent = send(new_fd, chptr, 1, 0)) == -1)
                {
                    perror("send");
                    exit(1);
                }

            }

            fin.close();
        }

        cout<<"Image sent from Server \n";

        close(new_fd);
        close(sock_fd);


    return 0;
}
