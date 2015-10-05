//This is a Client that takes takes in a JSON Object and broadcasts it to the Network (255.255.255.255) through UDP Datagrams

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include "json/json.h"
#define MYPORT 1111 // the port users will be connecting to


using namespace std;

int main (int argc, char* argv[])
{
    int sockfd, bytes_sent, broadcast=1, i=0, size;
    char ch,*chptr, *hostIPAddr = new char, *broadcastAddr = new char;


    sockaddr_in server_addr; // connector’s address information
    sockaddr_in hostIP;
    socklen_t hostIPlen = sizeof(hostIP);


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

    cout<< endl<< "My IP Address is: "<<hostIPAddr<<endl;
    cout<< "Enter Broadcast Address: ";
    cin>> broadcastAddr;

    // get the host info
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    cout<<"\nSocket Created with FD = "<<sockfd<<endl;


    int broadcastsock;
    // this call is what allows broadcast packets to be sent:
    if ((broadcastsock = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast)) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }
    cout<<"\n Set socket operation successfully"<<endl;


    server_addr.sin_family = AF_INET;// host byte order
    server_addr.sin_port = htons(MYPORT); // short, network byte order
    server_addr.sin_addr.s_addr = inet_addr(broadcastAddr);
    memset(&(server_addr.sin_zero), '\0', 8); // zero the rest of the struct


    /* Create JSON object with required values */
    cout << "\nCreating JSON object...<<endl";
    Json::Value JSONObject;
    string hostIPAddress(hostIPAddr);

    string firstName = "firstName";
    string lastName = "lastName";
    string gitUserName = "gitUserName";
    string currentIP = "currentIP";

    JSONObject[firstName] = "Kartic";
    JSONObject[lastName] = "Bhargav";
    JSONObject[gitUserName] = "karticbhargav";
    JSONObject[currentIP] = hostIPAddress;

    cout << endl << JSONObject << endl;

    /* Write JSON object to file */
    ofstream jsonFileWrite;

    jsonFileWrite.open("/home/karticbhargav/Identity.json", ios_base::trunc);
    jsonFileWrite << JSONObject << endl;
    jsonFileWrite.close();


    ifstream fin ("/home/karticbhargav/Identity.json");

    if (fin.is_open())
    {
        fin.seekg(0,ios::end);		//get file size
        size = fin.tellg();            //
        fin.seekg(0,ios::beg);         //

        chptr = new char[size+1];

        fin.read(chptr, size);
        fin.close();
    }

    chptr[size+1] = '\0';

    int len = strlen(chptr);

    if((bytes_sent = sendto(sockfd, chptr, len, 0, (sockaddr *)&server_addr, sizeof(sockaddr))) == -1){
        perror("sendto");
        exit(1);
    }

    fin.close();
    cout<<endl<<"JSON File Broadcasted"<<endl;

    close(sockfd);

    return 0;


}
