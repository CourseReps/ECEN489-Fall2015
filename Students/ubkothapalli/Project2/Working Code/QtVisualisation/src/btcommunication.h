#ifndef BTCOMMUNICATION_H
#define BTCOMMUNICATION_H

#include <qbluetoothserviceinfo.h>

#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class btcommunication: public QObject
{
    Q_OBJECT

public:
     explicit btcommunication(QObject *parent = 0);
     ~btcommunication();

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

#endif // BTCOMMUNICATION_H

