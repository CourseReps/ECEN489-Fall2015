// StreamSocket_Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

SOCKET s;
WSADATA w;
HWND hwnd;
SOCKADDR_IN addr; // The address structure for a TCP socket

//LISTENONPORT – Listens on a specified port for incoming connections 
//or data
void ListenOnPort(int portno)
{
    int error = WSAStartup(0x0202, &w);   // Fill in WSA info
    int bind_retval = 0;

    if (error)
    {
        cerr << "\nWinsock not initiated...";
        //return false; //For some reason we couldn't start Winsock
    }

    if (w.wVersion != 0x0202) //Wrong Winsock version?
    {
        WSACleanup();
        cerr << "\nWrong Winsock version...";
    }

    addr.sin_family = AF_INET;      // Address family
    addr.sin_port = htons(portno);   // Assign port to this socket

                                     //Accept a connection from any IP using INADDR_ANY
                                     //You could pass inet_addr("0.0.0.0") instead to accomplish the 
                                     //same thing. If you want only to watch for a connection from a 
                                     //specific IP, specify that //instead.
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

    if (s == INVALID_SOCKET)
    {
        cerr << "\nError in creating Socket...";
        //return false; //Don't continue if we couldn't create a //socket!!
    }

    bind_retval = bind(s, (LPSOCKADDR)&addr, sizeof(addr));
    if (SOCKET_ERROR == bind_retval)
    {
        //We couldn't bind (this will happen if you try to bind to the same  
        //socket more than once)
        cerr << "\nSocket bind Error...";
        //return false;
    }

    WSAAsyncSelect(s, hwnd, 1045, FD_READ | FD_CONNECT | FD_CLOSE);

    //Now we can start listening (allowing as many connections as possible to  
    //be made at the same time using SOMAXCONN). You could specify any 
    //integer value equal to or lesser than SOMAXCONN instead for custom 
    //purposes). The function will not //return until a connection request is 
    //made

    listen(s, SOMAXCONN);

    //Don't forget to clean up with CloseConnection()!
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection()
{
    //Close the socket if it exists
    if (s)
        closesocket(s);

    WSACleanup(); //Clean up Winsock
}

int main()
{
    int server_port;
    bool retval = false;

    cout << "Server Communication\n\nEnter server port: ";
    cin >> server_port;

    ListenOnPort(server_port);

    cout << "\nWaiting for message...\n";
    
    while (1)
    {
        SOCKET s1;
        int addrSize = sizeof(addr);
        int recv_retval = 0;
        int send_retval = 0;
        int handshake;
        int output = 1;

        s1 = accept(s, (struct sockaddr *) &addr, &addrSize);
        if (INVALID_SOCKET == s1)
        {
            cerr << "\nError in connection. Exiting...";
            break;
        }

        recv_retval = recv(s1, (char *)&handshake, sizeof(handshake), 0); //Get the text
        if (recv_retval < 0)
        {
            cerr << "\nError in Handshaking...";
            break;
        }
        else
        {
            cout << "\nReceived value: " << handshake;
        }
        
        if (handshake > 650)
        {
            output = 2;
        }
        else
        {
            output = 1;
        }

        if ((send_retval = send(s1, (char *)&output, sizeof(output), 0)) == -1)
        {
            cerr << "\nExiting...";
            CloseConnection();
            _getch();
            return 0;;
        }
        else
        {
            cout << "\nSent value: " << output;
        }
    }

    cout << "\nClosing connection...\n";

    CloseConnection();

    _getch();
    return 0;
}

