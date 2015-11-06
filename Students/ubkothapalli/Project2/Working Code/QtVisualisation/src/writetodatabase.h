#ifndef WRITETODATABASE_H
#define WRITETODATABASE_H

#include <QMainWindow>
#include <QtCore/QObject>

#include <qbluetoothdeviceinfo.h>
#include <qbluetoothserviceinfo.h>
#include <QBluetoothSocket>
#include <qbluetoothservicediscoveryagent.h>

class writetodatabase: public QObject
{
      Q_OBJECT
public:
   explicit writetodatabase(QObject *parent = 0);
    ~writetodatabase();

    void startDeviceDiscovery(const QBluetoothAddress &localAdapter);
    void initializedb();

private slots:

      void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
      void connected(const QString &name);
      void addtodb(const QString &data);
      void disConnected();


public slots:

      void startDb();
      void sendSetPoint(const QString setpoint);

signals:

    void sendMessage(const QString  &msg);
    void finished();

private:
    QBluetoothSocket *socket;
    QBluetoothServiceDiscoveryAgent *discoveryAgent;
    QBluetoothServiceInfo m_service;
};

#endif // WRITETODATABASE_H



