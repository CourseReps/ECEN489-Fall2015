#include "btthread.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#define CLIENTS_NUMBER 1
#define MYPORT 1111
#define BACKLOG 10

using namespace std;
sockaddr_in clientaddr[5];

btThread::btThread(QObject *parent) : QThread(parent)
{

}



void btThread::establishConnection ()
{
    //Fetching the IP Address
     sockaddr_in hostIP, myaddr;
//     socklen_t hostIPlen = sizeof(hostIP);
//     int sock = socket(AF_INET, SOCK_DGRAM, 0);
//     const char* kGoogleDnsIp = "8.8.8.8";
//     uint16_t kDnsPort = 53;
//     struct sockaddr_in serv;
//     memset(&serv, 0, sizeof(serv));
//     serv.sin_family = AF_INET;
//     serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
//     serv.sin_port = htons(kDnsPort);
//     int err = connect(sock, (const sockaddr*) &serv, sizeof(serv));
//     if(getsockname(sock, (sockaddr*) &hostIP, &hostIPlen) == -1){
//       perror("getsockname");
//     }
//     hostIPAddr = inet_ntoa (hostIP.sin_addr);
     int yes =1;

     hostIPAddr = "10.202.48.97";



     if((mainsocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
     {
         perror("socket");
         exit(1);

     }


     if (setsockopt(mainsocket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
         perror("setsockopt");
         exit(1);
     }


     myaddr.sin_family = AF_INET;
     myaddr.sin_port = htons(MYPORT);
     myaddr.sin_addr.s_addr = inet_addr(hostIPAddr);
     memset(&(myaddr.sin_zero), '\0', 8);



     if(bind(mainsocket, (sockaddr *)&myaddr, sizeof(sockaddr)) == -1)
     {
         perror("bind");
         exit(1);

     }

}

bool btThread::connectToClient (int i)
{
    socklen_t sin_size;
    struct sigaction sa;

    if((listen(mainsocket, BACKLOG)) == -1)
    {
        perror("listen");
        exit(1);

    }


    sin_size = (socklen_t)sizeof(sockaddr_in);

    if((sockfd[i] = accept(mainsocket, (sockaddr *)&clientaddr[i], &sin_size)) == -1)
    {
        perror("accept");
        exit(1);
    }

        cout<<sockfd[i]<<endl;
        cout<<"Server: Connected to: "<< inet_ntoa(clientaddr[i].sin_addr)<<endl;
}

void btThread::run()
{
    QString dest = "00:12:09:13:99:42";
    char *chptr = new char, ch;
    int bytes_sent, bytes_received, len;
    QString val;
    char* get_data = "get_data";
    QFile file;

    QJsonValue JIRRange, JPumpRate, JFlowRate, JSolenoidState, JID, JTimestamp, JCurrentIP;

    QJsonObject sett2;
    QJsonDocument d;



    establishConnection();

    for (int i=0; i<5; i++)
    {
            //Create sock_fd[i] and server_addr[i]
        if (connectToClient(i) == false)
        {
            qDebug() << "Connection failed to Client: " <<i;
        }
    }

    while(1)
    {
        QMutex mutex;

        mutex.lock();



        if(this->btThreadStop)
        {
            break;
        }

        mutex.unlock();

        for (int j=0; j<5; j++)
        {
            if((bytes_sent = send(sockfd[j], get_data, 9, 0)) == -1){
                perror("send");
                exit(1);
            }


            ofstream fout ("/home/karticbhargav/ClientIdentity.json");


            if ((bytes_received = recv(sockfd[j], chptr, 1000000, 0)) == -1)
            {
                perror("recv");
                exit(1);
            }


            len = strlen(chptr);
            for (int i=0; i< len ; i++)
            {
                ch = chptr[i];
                fout.put(ch);
            }

            fout.close();

            file.setFileName("/home/karticbhargav/ClientIdentity.json");
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            val = file.readAll();
            file.close();
            qWarning() << val;
            d = QJsonDocument::fromJson(val.toUtf8());
            sett2 = d.object();

            JID = sett2["ID"];
            ID = JID.toString(); //JIRRange, JPumpRate, JFlowRate, JSolenoidState, JID, JTimestamp, JCurrentIP

            JIRRange = sett2["IRRange"];
            IRRange = JID.toDouble();

            JPumpRate = sett2["PumpRate"];
            PumpRate = JID.toDouble();

            JFlowRate = sett2["FlowRate"];
            FlowRate = JID.toDouble();

            JSolenoidState = sett2["SolenoidState"];
            SolenoidState = JID.toBool();

            JTimestamp = sett2["Timestamp"];
            Timestamp = JID.toInt();

            JCurrentIP = sett2["CurrentIP"];
            CurrentIP = JID.toString();


            emit setParameterValues(IRRange, PumpRate, FlowRate, SolenoidState, ID, Timestamp, CurrentIP);

        }



        msleep(1000);
    }

}
