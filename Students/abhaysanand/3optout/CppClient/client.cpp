// StreamSocket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

SOCKET s; //Socket handle
HWND hwnd, hStatus;
SOCKADDR_IN target; //Socket address information
int connect_retval = 0;

          //CONNECTTOHOST – Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress)
{
    //Start up Winsock…
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return false;

    //Did we get the right Winsock version?
    if(wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return false;
    }

    //Fill out the information needed to initialize a socket…

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons(PortNo); //Port to connect on
    target.sin_addr.s_addr = inet_addr(IPAddress); //Target IP

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (s == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }

    //Try connecting...

    connect_retval = connect(s, (SOCKADDR *)&target, sizeof(target));

    if (SOCKET_ERROR == connect_retval)
    {
        return false; //Couldn't connect
    }
    else
        return true; //Success

    WSAAsyncSelect(s, hwnd, 1045, FD_READ | FD_CONNECT | FD_CLOSE);
    //Switch to Non-Blocking mode

    //SendMessage(hStatus, WM_SETTEXT, 0, (LPARAM)"get_photo");
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
    bool retval = false;
    int Port;
    int send_retval = 0;
    int screenshot_size = 0;
    string IPaddress;
    char screenshot_char[1];

    cout << "Client Communication\n\nEnter server IP: ";
    getline(cin, IPaddress);

    cout << "\nEnter server Port: ";
    cin >> Port;

    cout << "\nAttempting to connect to server: " << IPaddress << " ...\n";

    retval = ConnectToHost(Port, &IPaddress[0]);

    if (true == retval)
    {
        cout << "\nConnection successful...\n";
    }
    else
    {
        cerr << "\nConnection failed...\nClosing connection...";
        CloseConnection();
        _getch();
        return 0;
    }

    char getPhoto[10] = "get_photo";

    cout << "\n Initiating Handshaking...";

    send_retval = send(s, (const char *)getPhoto, sizeof(getPhoto), 0);
    if (SOCKET_ERROR == send_retval)
    {
        cerr << "\nError in transmission...\nClosing connection...";
        CloseConnection();
        _getch();
        return 0;
    }

    cout << "\nReceiving screenshot...";

    //FILE *screenshot = fopen("screenshot.jpg", "w+");
    ofstream screenshot("screenshot.jpg", ios_base::trunc | ios_base::binary);
    cout << screenshot.good();
    //screenshot.open("screenshot.jpg", ios_base::trunc | ios_base::binary);
    screenshot.seekp(0);
    int len = 1;

    while (len > 0)
    {
        screenshot_size++;
        if ((len = recv(s, screenshot_char, sizeof(char), 0)) == -1)
        {
            cerr << "\nError in connection. Exiting...";
        }
        screenshot.put(*screenshot_char);
    }
    if (screenshot.eof())
    {
        cout << "\n Image reception complete...\n" << screenshot_size << " bytes received";
    }
    screenshot.close();

    cout << "\nClosing connection...\n";

    CloseConnection();

    _getch();
    return 0;
}

