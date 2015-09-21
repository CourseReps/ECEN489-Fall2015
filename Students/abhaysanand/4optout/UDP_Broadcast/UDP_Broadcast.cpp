// UDP_Broadcast.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
string ip;

int ipaddr()
{
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        cerr << "Error " << WSAGetLastError() <<
            " when getting local host name." << endl;
        return 1;
    }
    //cout << "Host name is " << ac << "." << endl;

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
        cerr << "Yow! Bad host lookup." << endl;
        return 1;
    }

    struct in_addr addr;
    memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
    ip = inet_ntoa(addr);

    return 0;
}

SOCKET sock;

struct sockaddr_in Sender_addr;
int len = sizeof(struct sockaddr_in);

int main()
{
    int port;

    cout << "\nUDP Broadcast\nEnter port no: ";
    cin >> port;

    cout << "\nEstablishing UDP socket connection...";

    /* Establish Socket and UDP connection */
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    char broadcast = 'a';

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        cerr << "\nBroadcast socket creation error...\nExiting...";
        _getch();
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "\nConnection established...";
    }

    Sender_addr.sin_family = AF_INET;
    Sender_addr.sin_port = htons(port);
    Sender_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    /* Get IP address */
    (void)ipaddr();

    /* Create JSON object with required values */
    cout << "\nCreating JSON object...";
    Json::Value objJSON;

    string firstName = "firstName";
    string lastName = "lastName";
    string gitUserName = "gitUserName";
    string currentIP = "currentIP";

    objJSON[firstName] = "Abhay Shankar";
    objJSON[lastName] = "Anand";
    objJSON[gitUserName] = "abhaysanand";
    objJSON[currentIP] = ip;
    
    cout << endl << objJSON << endl;

    /* Write JSON object to file */
    ofstream jsonFileWrite;

    jsonFileWrite.open("Broadcast.json", ios_base::trunc | ios_base::binary);
    jsonFileWrite << objJSON << endl;
    jsonFileWrite.close();

    /* Read JSON object from file for sending */
    ifstream jsonFileRead;
    char readBuffer;
    char *readBuffer_p = new char;
    uint16_t filesize = 0;
    int sendto_retval;

    jsonFileRead.open("Broadcast.json", ios_base::in | ios_base::binary);

    cout << "\nBroadcasting JSON object on port: "<< port;
    
    while (!jsonFileRead.eof())
    {
        filesize++;
        jsonFileRead.get(readBuffer);
        readBuffer_p = &readBuffer;
        sendto_retval = sendto(sock, readBuffer_p, sizeof(char), 0, (sockaddr *)&Sender_addr, len);
        if (sendto_retval < 0)
        {
            cerr << "Broadcast error...\nExiting...";
            _getch();
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    }

    cout << "\nBroadcast successful. Transferred: " << filesize << " bytes";

    cout << "\nClosing connection...\nExiting...";

    _getch();
    closesocket(sock);
    WSACleanup();
    return 0;
}

