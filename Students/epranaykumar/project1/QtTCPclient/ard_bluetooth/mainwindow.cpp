#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tcpsocket.h"

#include <qbluetoothserver.h>
#include <qbluetoothsocket.h>
#include <QBluetoothSocket>

#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
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

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include "json/json.h"

#include  <unistd.h>
#include <time.h>


//QVector<double>  rec_data(6);
QVector<QString>  rec_data(6);
QSqlDatabase db;
int dt_ind=0;

void MainWindow::startDeviceDiscovery(const QBluetoothAddress &localAdapter)
{

    // Create a discovery agent and connect to its signals
    discoveryAgent = new QBluetoothServiceDiscoveryAgent(localAdapter);

    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

    // Start a discovery
    discoveryAgent->start();

    qDebug() << "\tService started";

}

// In your local slot, read information about the found devices
void MainWindow::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered service on" << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();

    if(serviceInfo.device().name() == "linvor"){

           // Connect to service
          socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
          qDebug() << "Create socket";
          socket->connectToService(serviceInfo);
          qDebug() << "ConnectToService done";

          if (discoveryAgent->isActive())
              discoveryAgent->stop();

          qDebug() << "discovery agent stopped";

          // qDebug("sent connected signal to teensy");

          connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
          connect(socket, SIGNAL(connected()), this, SLOT(connected()));
          connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    }


}

//! [readSocket]
void MainWindow::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {

             QString line = socket->readLine();
             line.remove("\r\n", Qt::CaseInsensitive);
             //qDebug()<< line;

             rec_data[dt_ind]= line;
             qDebug()<<   rec_data[dt_ind];


             if(  rec_data[dt_ind] == "200000"){
                    dt_ind=0;
                    addtodb();
                    qDebug( "calledaddtodb" );                   
             }

             else  dt_ind++;
    }
}



void MainWindow::connected()
{
    emit connected(socket->peerName());
}

/*void MainWindow::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}*/


void MainWindow::initializedb(){

      // assign the appropriate driver to the database
        db = QSqlDatabase::addDatabase("QSQLITE");


       // open the specified database
        db.setDatabaseName( "/home/pranaykumar/data.db");

        if(!db.open()){
             qDebug("Failed to open the database");
        }
        else{
             qDebug("Connected to the database");

            QString stmt =QString("CREATE TABLE DATA (DeviceID TEXT,Timestamp TEXT, IR_sensor TEXT, RelayState TEXT,  Voltage TEXT, Flowrate TEXT);");
            qDebug() <<"Query:"<< stmt;
            QSqlQuery query;
            query.prepare(stmt);

            if( !query.exec() )
                qDebug() << query.lastError();
            else
                qDebug( "Table created successfully" );

        }

}

void MainWindow::addtodb(){


     qDebug( "inaddtodb" );

            long int timestamp =time(NULL);

            QString stmt1 =QString("INSERT INTO DATA VALUES('team2',' "+ QString("%1").arg(timestamp)+ " ',' " + rec_data[0] + " ',' " +  rec_data[1]+ " ',' " +
                                 rec_data[2] + " ',' " + rec_data[3]+ " ') ");
            qDebug() <<"Query:"<< stmt1;
            QSqlQuery query1;
            query1.prepare(stmt1);

            if( !query1.exec() )
                qDebug() << query1.lastError();
            else
                qDebug( "inserted row successfully" );



}


 void MainWindow::sendDatabase(){
 }



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initializedb(); // initialize the database

    // start tcp socket
    QThread* thread = new QThread;
    tcpsocket = new TCPSocket();


     tcpsocket->moveToThread(thread);
    connect(thread, SIGNAL(started()),  tcpsocket, SLOT(ConnecttoTCPserver()));
    connect(tcpsocket, SIGNAL(finished()), thread, SLOT(quit()));
    connect(tcpsocket, SIGNAL(finished()), tcpsocket, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();




    QBluetoothLocalDevice localDevice;
    QString localDeviceName;

    qDebug()<<localDevice.isValid();

    // Check if Bluetooth is available on this device
    if (localDevice.isValid()) {

        // Turn Bluetooth on
        localDevice.powerOn();

        // Read local device name
        localDeviceName = localDevice.name();
        qDebug()<<localDeviceName;

         QBluetoothAddress  localAdapter = localDevice.address();
         qDebug()<<localAdapter;

        // Make it visible to others
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        // Get connected devices
        QList<QBluetoothAddress> remotes;
        remotes = localDevice.connectedDevices();
        qDebug()<<remotes;

        startDeviceDiscovery(localAdapter);

        qDebug("started service discovery");

    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
