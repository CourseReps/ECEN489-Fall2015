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
        int accept_retval = 0;
        int send_retval = 0;
        int screenshot_size = 0;
        int send_length = 0;
        char handshake[10];
        char ch;
        char *screenshot_char = new char;
        char test = 'c';

        memset(handshake, 0, sizeof(handshake)); //Clear the handshake buffer

        s1 = accept(s, (struct sockaddr *) &addr, &addrSize);
        if (INVALID_SOCKET == s1)
        {
            cerr << "\nError in connection. Exiting...";
            break;
        }

        recv(s1, handshake, sizeof(handshake), 0); //Get the text
        if (memcmp(handshake, "get_photo", 9) == 0)
        {
            cout << "\nHandshaking successful...";
        }
        else
        {
            cerr << "\nError in Handshaking...";
            break;
        }

        //TakeScreenShot("screenshot.jpg");

        ifstream screenshot;
        screenshot.open("../screenshot.jpg", ios_base::in | ios_base::binary);

        int tx = 0;
        cout << "\nTransmitting image to Client...";
        screenshot.seekg(0);
        //screenshot.get(ch);

        while (!screenshot.eof())
        {
            screenshot_size++;
            screenshot.get(ch);
            screenshot_char = &ch;
            if ((tx = send(s1, screenshot_char, 1, 0)) == -1)
            {
                cerr << "\nExiting...";
                screenshot.close();
                CloseConnection();
                _getch();
                return 0;;
            }
        }

        if (screenshot.eof())
        {
            cout << "\n Transmission complete...\n" << screenshot_size << " bytes transferred";
            screenshot.close();
            break;
        }
    }

    cout << "\nClosing connection...\n";

    CloseConnection();

    _getch();
    return 0;
}

