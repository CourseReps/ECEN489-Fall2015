#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "sqlite3.h"

using namespace std;

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status, len=1;
    char dest[18] = "00:12:09:13:99:42";
    char get_data[9] = "get_data";
    // allocate a socket

    cout << "\nConnecting to device: 'team1'...";

    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));


    if(status)
    {
        cout <<"\nFailed to connect the device!";
        return -1;
    }
    else
    {
        cout << "\nConnected to device...\n\nInitiating handshake...";
    }

    /* Initiate handshake */
    status = write(s, get_data, sizeof(get_data));

    if(status > 0)
    {
        cout << "\nHandshaking message sent. Waiting for response...\n";
    }
    else
    {
        cout << "\nFailed to send handshaking message!";
        return -1;
    }

    char buf;
    stringstream buffer;

    /*len = read(s,buf,1000);
    buffer << buf;
    cout << endl << "len = " << len << buffer;*/

    while((len == 1) && (buf != '\n'))
    {
        len = read(s,&buf,1);
        buffer << buf;
        //cout << endl << "len = " << len << buffer.str();
    }

    string readBuf;
    readBuf = buffer.str();

    //cout << readBuf;

    istringstream ss(readBuf);
    string token;

    //int

    while(getline(ss, token, ','))
    {
        cout << endl << token;
    }
}
