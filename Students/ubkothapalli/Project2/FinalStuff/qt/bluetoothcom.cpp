#include "bluetoothcom.h"
#include <qbluetoothsocket.h>

#include <qbluetoothserviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>

#include <qbluetoothaddress.h>
#include <qbluetoothuuid.h>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileInfo>
#include <QtCore>

#include  <unistd.h>

#include <time.h>




bluetoothCom::bluetoothCom(QObject *parent)
    :   QObject(parent), socket(0)
{

}

bluetoothCom::~bluetoothCom()
{
    stopCommunication();
}


void bluetoothCom::startCommunication(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}



void bluetoothCom::stopCommunication()
{
    delete socket;
    socket = 0;
}



void bluetoothCom::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
             QString line = socket->readLine();
             line.remove("\r\n", Qt::CaseInsensitive);
             qDebug()<< line;
             emit dataReceived(line);
            }
}



void bluetoothCom::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8();
    qint64 aa=socket->write(text);
    qDebug() << aa;
}



void bluetoothCom::connected()
{
     qDebug()<< "connected";
    emit connected(socket->peerName());
}



