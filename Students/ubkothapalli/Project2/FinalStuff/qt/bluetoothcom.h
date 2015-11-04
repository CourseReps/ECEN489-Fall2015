#ifndef BLUETOOTHCOM_H
#define BLUETOOTHCOM_H

#include <qbluetoothserviceinfo.h>

#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class bluetoothCom: public QObject
{
    Q_OBJECT

public:
     explicit bluetoothCom(QObject *parent = 0);
     ~bluetoothCom();

    void startCommunication(const QBluetoothServiceInfo &remoteService);
    void stopCommunication();

public slots:
    void sendMessage(const QString &message);

signals:
    void dataReceived(const QString &data);
    void connected(const QString &name);
    void disconnected();

private slots:
    void readSocket();
    void connected();

private:
    QBluetoothSocket *socket;

};

#endif // BLUETOOTHCOM_H

