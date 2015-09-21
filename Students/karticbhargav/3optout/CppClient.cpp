#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream>
#define MYPORT 3490 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold



using namespace std;

int main (int argc, char* argv[])
{
    int sockfd, bytes_sent, bytes_received, len;
    sockaddr_in server_addr; // connector’s address information
    char ch, *chptr, cmd[] = "get_photo";
    len = strlen(cmd);

    if (argc != 2) {
        fprintf(stderr,"usage: client ServerAddress\n");
        exit(1);
    }


    // get the host info
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;// host byte order
    server_addr.sin_port = htons(MYPORT); // short, network byte order
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    memset(&(server_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if (connect(sockfd, (sockaddr *)&server_addr,sizeof(sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }


    //cout<<"Client Connected to: "<< argv[1]<<endl;
    if((bytes_sent = send(sockfd, cmd, len, 0)) == -1){
            perror("send");
            exit(1);
    }

    cout<<bytes_sent<<endl;

    ofstream fout ("/home/karticbhargav/client_image.jpeg");


    while ((bytes_received = recv(sockfd, chptr, 1, 0)) != -1){

        ch = *chptr;
        fout.put(ch);

    }

    cout<< "Received image file \n";
    close(sockfd);


}
