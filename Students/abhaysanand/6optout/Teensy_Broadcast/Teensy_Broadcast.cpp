// UDP_Broadcast.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
string ip;

HANDLE hSerial;

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

    cout << "\nReading Teensy data from COM3...";

    string com = "COM3";
    //wstring comTemp = wstring(com.begin(), com.end());
    //LPCWSTR comPort = comTemp.c_str();

    /* Configure COM Port 3 for reading */
    hSerial = CreateFile(com.c_str(), GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    /* Set all COM3 port parameters */
    DCB dcbSerialParams = { 0 };

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    /* COnfigure timeouts for COM3 */
    COMMTIMEOUTS timeouts = { 0 };

    timeouts.ReadIntervalTimeout = 100;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 20;

    SetCommTimeouts(hSerial, &timeouts);

    /* Read from COM3 */
    char volt[5];
    DWORD bytesRead = 0;

    ReadFile(hSerial, &volt, sizeof(volt), &bytesRead, NULL);

    CloseHandle(hSerial);

    string photoVolt = (string)volt;

    photoVolt = photoVolt.substr(0, photoVolt.find_first_of('\r', 0));
    uint16_t voltValue = atoi(photoVolt.c_str());

    cout << "\nPhotoresistor data = " << voltValue;

    cout << "\nBroadcasting on port: 1111";
    //cin >> port;
    port = 1111;

    cout << "\nEstablishing UDP socket connection...";

    /* Establish Socket and UDP connection */
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int optval = true;
    //char broadcast = 'a';

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(int)) < 0)
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

    /* Get Epoch time */
    time_t epoch = time(NULL);

    /* Create JSON object with required values */
    cout << "\nCreating JSON object...";
    Json::Value objJSON;

	ostringstream convertVolt, convertTime;
	
	convertVolt << voltValue;
	convertTime << epoch;
	
    string DeviceID = "DeviceID";
    string DeviceType = "DeviceType";
    string Data = "Data";
    string TimeStamp = "TimeStamp";
    string currentIP = "currentIP";

    objJSON[DeviceID] = "ASA007";
    objJSON[DeviceType] = "Photosensor";
    objJSON[Data] = convertVolt.str();
    objJSON[TimeStamp] = convertTime.str();
    objJSON[currentIP] = ip;

    //cout << endl << objJSON << endl;

    /* Write JSON object to file */
    ofstream jsonFileWrite;

    jsonFileWrite.open("Broadcast.json", ios_base::trunc);
    jsonFileWrite << objJSON << endl;
    jsonFileWrite.close();

    /* Read JSON object from file for sending */
    ifstream jsonFileRead;
    char *readBuffer_p = new char;
    size_t filesize = 0;
    int sendto_retval;

    jsonFileRead.open("Broadcast.json", ios_base::in);

    cout << "\nBroadcasting JSON object on port: "<< port;

    stringstream strStream;
    strStream << jsonFileRead.rdbuf();//read the file
    string jsonStr = strStream.str();//str holds the content of the file

    cout << endl << jsonStr << endl;

    filesize = jsonStr.size();

    sendto_retval = sendto(sock, jsonStr.c_str(), filesize, 0, (sockaddr *)&Sender_addr, len);
    if (sendto_retval < 0)
    {
        cerr << "Broadcast error...\nExiting...";
        _getch();
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "\nBroadcast successful. Transferred: " << filesize << " bytes";

    cout << "\nClosing connection...\nExiting...";

    _getch();
    closesocket(sock);
    WSACleanup();
    return 0;
}

