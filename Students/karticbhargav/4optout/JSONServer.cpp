#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>
#include "json/json.h"
#include "sqlite3/sqlite3.h"


using namespace std;

#define MYPORT 1111 // the port users will be connecting to

void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}


int main(int argc, char *argv[])
{
    int sockfd;
    sockaddr_in myaddr, clientAddr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1, bytes_received;

    sockaddr_in hostIP;
    socklen_t hostIPlen = sizeof(hostIP);

    char ch, *chptr = new char [200], *hostIPAddr = new char;

/*
    if (argc!=2)
    {
        fprintf(stderr,"usage: Server ClientAddress\n");
        exit(1);
    }
    */

    //Fetching the IP Address
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    const char* kGoogleDnsIp = "8.8.8.8";
    uint16_t kDnsPort = 53;
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(kDnsPort);
    int err = connect(sock, (const sockaddr*) &serv, sizeof(serv));
    if(getsockname(sock, (sockaddr*) &hostIP, &hostIPlen) == -1){
        perror("getsockname");
    }

    hostIPAddr = inet_ntoa (hostIP.sin_addr);



    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);

    }


    if (setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    cout<<endl<<hostIPAddr<<endl;

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(MYPORT);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(myaddr.sin_zero), '\0', 8);


    if(bind(sockfd, (sockaddr *)&myaddr, sizeof(sockaddr)) == -1)
    {
        perror("bind");

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
    cout<<"Assigned Server Details successfully\n";


    //cout<<"Server: Connected to: "<< inet_ntoa(clientAddr.sin_addr)<<endl;


    if ((bytes_received = recvfrom(sockfd,chptr, 1000000, 0,(struct sockaddr *)&clientAddr, &sin_size)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }

    ofstream fout ("/home/karticbhargav/ClientIdentity.json");
    int len = strlen(chptr);
    cout<<len<<endl;
    cout<<chptr<<endl;
    for (int i=0; i< len ; i++)
    {
        ch = chptr[i];
        fout.put(ch);
    }
    fout.close();
    cout<<"File received from client \n";

    close(sockfd);


    // Parse data.
    Json::Value client_msg;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(chptr, client_msg);

    // store to Sqlite database
    if (parsingSuccessful)
    {
        string firstName = client_msg["firstName"].asString();
        string lastName = client_msg["lastName"].asString();
        string gitUserName = client_msg["gitUserName"].asString();
        string currentIP = client_msg["currentIP"].asString();

        sqlite3 *db;
        char *err_msg = 0;
        sqlite3_stmt *stmt;

        int rc = sqlite3_open("/home/karticbhargav/workspace/data.db", &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return 1;
        }

        // create table
        char sql[200] ="CREATE TABLE Client_IP( firstName TEXT, lastName TEXT, gitUserName TEXT,currentIP TEXT);";
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        //insert data
        char sql4[200];

        string statement = " INSERT INTO Client_IP VALUES(' "+firstName+" ',' " + lastName + " ',' "+ gitUserName + " ',' " + currentIP + " ') ";
        strcpy(sql4, statement.c_str());

        rc=sqlite3_exec(db,sql4,0,0,&err_msg);

        if( rc!=SQLITE_OK ){
            fprintf(stderr, "\n SQL error: %s   %s\n", err_msg,sql4);
            sqlite3_free(err_msg);
        }


        sqlite3_close(db);
    }



    return 0;
}
