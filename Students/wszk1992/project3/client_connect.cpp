#include "client_connect.h"

static int sockfd = 0;

QString client_connect(QString serverIP, bool *conn_state)
{
    int bytes_received;
    static sockaddr_in server_addr = {0}; // connector’s address information

    if(false == *conn_state)
    {
        // get the host info
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            *conn_state = false;
            qDebug() << "\nSocket creation ERROR...";
            close(sockfd);
            return "Socker Error";
        }
        else
        {
            *conn_state = true;
        }

        server_addr.sin_family = AF_INET;// host byte order
        server_addr.sin_port = htons(1111); // short, network byte order
        server_addr.sin_addr.s_addr = inet_addr(serverIP.toStdString().c_str());
        memset(&(server_addr.sin_zero), '\0', 8); // zero the rest of the struct

        if (connect(sockfd, (sockaddr *)&server_addr,sizeof(sockaddr)) == -1)
        {
            *conn_state = false;
            qDebug() << "\nConnection ERROR...";
            close(sockfd);
            return "Conn. Error";
        }
        else
        {
            *conn_state = true;
        }
    }

    char cmd[9];

    if((bytes_received = recv(sockfd, &cmd, sizeof(cmd), 0)) == -1)
    {
        qDebug() << "Authentication failure";
        return "Auth. Failure";
    }

    if(strcmp(cmd, "get_data") == 0)
    {
        QFile file("sensor.json");
        file.open(QFile::ReadOnly | QFile::Text);

        QByteArray readFile = file.readAll();

        int bytes_sent;
        if((bytes_sent = send(sockfd, readFile.constData(), readFile.size(), 0)) == -1)
        {
            qDebug() << "\nSend json ERROR...";
            return "Send Error";
        }
        else
        {
            qDebug() << readFile.constData();
        }

        file.close();
        return "Send Success";
    }
    return "Waiting";
}

void client_close()
{
    close(sockfd);
}
