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
    sockaddr_in myaddr, server_addr, dummy_addr;
    int yes=1, bytes_received, bytes_sent;

    struct sigaction sa;

    sockaddr_in hostIP;
    socklen_t sin_size, hostIPlen = sizeof(hostIP);

    char ch, *chptr = new char [200], *hostIPAddr = new char, cmd[] = "get_data";
    int len = strlen(cmd);

    if (argc!=2)
    {
        fprintf(stderr,"usage: Client ServerAddress\n");
        exit(1);
    }


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


    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    cout<<endl<<"Client IP Address:"<<hostIPAddr<<endl;

    server_addr.sin_family = AF_INET;// host byte order
    server_addr.sin_port = htons(MYPORT); // short, network byte order
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    memset(&(server_addr.sin_zero), '\0', 8); // zero the rest of the struct


    sin_size = (socklen_t)sizeof(sockaddr_in);


    if ((bytes_sent = sendto(sockfd, cmd, len, 0, (sockaddr *) &server_addr, sizeof(sockaddr))) == -1) {
        perror("sendto");
        exit(1);
    }

    cout<<"Sent Command\n";


    if ((bytes_received = recvfrom(sockfd,chptr, 10000000, 0,(struct sockaddr *)&server_addr, &sin_size)) == -1)
    {
        perror("recvfrom");
        exit(1);
    }

    cout<<"Here's what was received:\n"<<chptr<<endl;

    ofstream fout ("/home/karticbhargav/ReceiveThermistorData.json");


    int length = strlen(chptr);
    for (int i=0; i< length ; i++)
    {
        ch = chptr[i];
        fout.put(ch);
    }
    fout.close();
    cout<<"\nFile received from Server \n";



    close(sockfd);


    // Parse data.
    Json::Value client_msg;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(chptr, client_msg);

    // store to Sqlite database
    if (parsingSuccessful)
    {
        string DeviceID = client_msg["DeviceID"].asString();
        string DeviceType = client_msg["DeviceType"].asString();
        string Data = client_msg["Data"].asString();
        string currentIP = client_msg["currentIP"].asString();
        string Timestamp = client_msg["Timestamp"].asString();

        int l = Data.size();
        char* DevTemp = new char[10];
        for (int j=0; j<l; j++)
        {
            DevTemp[j] = Data[j];
        }
        if (strcmp(DevTemp, "100") < 0) {
            cout << DeviceID << " is HOT!" << endl;
        }

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
        char sql[200] ="CREATE TABLE Client_IP( DeviceID TEXT, DeviceType TEXT, Data TEXT,currentIP TEXT, Timestamp TEXT);";
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        //insert data
        char sql4[200];

        string statement = " INSERT INTO Client_IP VALUES(' "+DeviceID+" ',' " + DeviceType + " ',' "+ Data + " ',' " + currentIP + " ',' "+ Timestamp +" ') ";
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
