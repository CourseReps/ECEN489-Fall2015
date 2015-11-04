#include "todatabase.h"
#include "bluetoothcom.h"
#include "ui_mainwindow.h"

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

#include  <unistd.h>
#include <time.h>


QVector<QString>  rec_data(6);

int dt_ind=0;

toDatabase::toDatabase(QObject *parent)
    :   QObject(parent)
{

}

toDatabase::~toDatabase()
{

}

void toDatabase::startDb(){

    initializedb(); // initialize the database

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

void toDatabase::startDeviceDiscovery(const QBluetoothAddress &localAdapter)
{

    // Create a discovery agent and connect to its signals
    discoveryAgent = new QBluetoothServiceDiscoveryAgent(localAdapter);

    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));

    // Start a discovery
    discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

// In your local slot, read information about the found devices
void toDatabase::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered service on" << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();

    m_service = serviceInfo;

    if(m_service.device().name() == "linvor"){

        if (discoveryAgent->isActive())
            discoveryAgent->stop();

        qDebug() << "discovery agent stopped";

        qDebug() << "Going to start comm with linvor";
        bluetoothCom *teensy = new bluetoothCom(this);

        connect(teensy, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
        connect(teensy, SIGNAL(disconnected()), this, SLOT(disConnected()));

        connect(teensy, SIGNAL(dataReceived(const QString)),this, SLOT( addtodb(const QString)));
        connect(this, SIGNAL(sendMessage(QString)), teensy, SLOT(sendMessage(QString)));

        qDebug() << "Start Communication";
        teensy->startCommunication(m_service);

    }

}



void toDatabase::connected(const QString &per)//const QString &per
{

       qDebug()<<per;
    // emit sendMessage("500");
}

void toDatabase::disConnected()//const QString &per
{
    emit  finished();
}



void toDatabase::initializedb(){

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
       // open the specified database
        db.setDatabaseName( "/home/uday/Arduino/Project_2_arduino/Project_2_Arduino/data.db");

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
         db.close();

}

void toDatabase::addtodb(const QString &data){


     qDebug( "inaddtodb" );

     rec_data[dt_ind]= data;
     qDebug()<< rec_data[dt_ind];

     if(rec_data[dt_ind] != "s")  dt_ind++;

    else{
            dt_ind=0;

            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
          // open the specified database
           db.setDatabaseName( "/home/uday/Arduino/Project_2_arduino/Project_2_Arduino/data.db");

           if(!db.open()){
                qDebug("Failed to open the database");
           }
           else{
                qDebug( "adding to database" );
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

                 db.close();
           }

                //emit sendMessage("1");


        }
}


void toDatabase::sendSetPoint(const QString setpoint){

     emit sendMessage(setpoint);

}
