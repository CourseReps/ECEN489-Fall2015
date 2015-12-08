#include "bluetooth_connect.h"

static int s = 0;

QString bluetooth_connect(QString dest_addr, QString message, bool *conn_state)
{
    static struct sockaddr_rc addr = { 0 };
    static int status = 0;
    static int len=1;

    // allocate a socket
    if(false == *conn_state)
    {
        cout << "\nConnecting to device: 'team1'...";

        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // set the connection parameters (who to connect to)
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_channel = (uint8_t) 1;
        str2ba( dest_addr.toStdString().c_str(), &addr.rc_bdaddr );

        // connect to server
        status = connect(s, (struct sockaddr *)&addr, sizeof(addr));


        if(status == -1)
        {
            *conn_state = false;
            close(s);
            cout <<"\nFailed to connect the device!";
            return NULL;
        }
        else
        {
            *conn_state = true;
            cout << "\nConnected to device...\n\nInitiating handshake...";
        }
    }

    /* Initiate handshake */
    int a = message.length() + 1;

    status = write(s, message.toStdString().c_str(), a);

    if(status > 0)
    {
        cout << "\nHandshaking message sent. Waiting for response...\n";
    }
    else
    {
        cout << "\nFailed to send handshaking message!";
        return NULL;
    }

    for(int i = 0; i < INT_MAX/10; i++)
    {

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
//        cout << endl << "len = " << buffer.str();
    }
//    cout << buffer.str();
    //close(s);

    QString readBuf = QString::fromStdString(buffer.str());

    return readBuf;
}

void bluetooth_close()
{
    close(s);
}
