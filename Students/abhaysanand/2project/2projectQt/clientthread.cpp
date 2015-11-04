#include "clientthread.h"
#include <client_connect.h>

clientThread::clientThread(QObject *parent) : QThread(parent)
{

}

void clientThread::run()
{
    QMutex mutex;

    mutex.lock();
    QString getServerIP = this->serverIP;
    mutex.unlock();

    static bool conn_state = false;
    while(1)
    {
        bool clientThreadStopLocal;

        mutex.lock();
        clientThreadStopLocal = this->clientThreadStop;
        mutex.unlock();

        if(clientThreadStopLocal)
        {
            conn_state = false;
            client_close();

            emit setLabelClient("Stopped");
            break;
        }

        QString label = client_connect(getServerIP, &conn_state);
        emit setLabelClient(label);
    }
}

