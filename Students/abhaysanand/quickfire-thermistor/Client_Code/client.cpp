// StreamSocket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

SOCKET s; //Socket handle
SOCKET s1;
WSADATA w;
HWND hwnd, hStatus;
SOCKADDR_IN target; //Socket address information
SOCKADDR_IN addr; // The address structure for a TCP socket
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

    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //Create socket
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

    s1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

    if (s1 == INVALID_SOCKET)
    {
        cerr << "\nError in creating Socket...";
        //return false; //Don't continue if we couldn't create a //socket!!
    }

    bind_retval = bind(s1, (LPSOCKADDR)&addr, sizeof(addr));
    if (SOCKET_ERROR == bind_retval)
    {
        //We couldn't bind (this will happen if you try to bind to the same  
        //socket more than once)
        cerr << "\nSocket bind Error...";
        //return false;
    }

    WSAAsyncSelect(s1, hwnd, 1045, FD_READ | FD_CONNECT | FD_CLOSE);

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
    bool retval = false;
    int Port;
    int send_retval = 0;
    int temp_size = 0;
    string IPaddress;
    char data_char[250];

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

    char getPhoto[9] = "get_data";

    cout << "\nInitiating Handshaking...";

    send_retval = send(s, (const char *)getPhoto, sizeof(getPhoto), 0);
    if (SOCKET_ERROR == send_retval)
    {
        cerr << "\nError in transmission...\nClosing connection...";
        CloseConnection();
        _getch();
        return 0;
    }

    cout << "\nReceiving Sensor Data...";

    //FILE *temp = fopen("temp.jpg", "w+");
    ofstream temp;
	ifstream jsonFileRead;
	int len = sizeof(addr);
	int recvfrom_retval;
	
	ListenOnPort(Port);
	
	recvfrom_retval = recvfrom(s, data_char, sizeof(data_char), 0, (sockaddr *)&addr, &len);
	
	if (recvfrom_retval == -1)
	{
		cerr << "\nError in connection. Exiting...";
		CloseConnection();
        _getch();
        return 0;
	}
	else
	{
		cout << "\nData received...";
	}
	
	string recvStr = (string)data_char;
	size_t recvStr_end = recvStr.find('}', 0) + 1;
	recvStr.erase(recvStr.begin() + recvStr_end, recvStr.end());
	cout << endl << recvStr << endl;
	size_t fileSize = recvStr.length();
	
	temp.open("Temp.json", ios_base::trunc | ios_base::binary);
	temp.write(recvStr.c_str(), fileSize);
	temp.close();
	
	cout << "\nReception from the source is completed. Received " << fileSize << " bytes.\n";

	Json::Value objJSONin;
	Json::Reader objJSONparse;
	
	jsonFileRead.open("Temp.json", ios_base::in | ios_base::binary);
	objJSONparse.parse(jsonFileRead, objJSONin, true);
	jsonFileRead.close();
	
	Json::StreamWriterBuilder builder;
	
	string Data = Json::writeString(builder, objJSONin["Data"]);
    Data.erase(Data.begin());
    Data.erase(Data.end() - 1, Data.end());
	
	int tempServer = atoi(Data.c_str());
	
	if (tempServer > 393)
	{
		cout << "\nHeat Gun found!!!";
	}
	else
	{
		cout << "\nHeat Gun not detected";
	}

    cout << "\nClosing connection...\n";

    CloseConnection();

    _getch();
    return 0;
}

