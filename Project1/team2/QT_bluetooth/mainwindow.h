#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothserviceinfo.h>
#include <QBluetoothSocket>
#include <qbluetoothservicediscoveryagent.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startDeviceDiscovery(const QBluetoothAddress &localAdapter);
    void addtodb();
    void initializedb();

private slots:
      void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
      void readSocket();
      void connected();

public slots:
  //  void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &sender, const QString &message);
    void connected(const QString &name);
    void disconnected();

private:
    Ui::MainWindow *ui;
    QBluetoothSocket *socket;
    QBluetoothServiceDiscoveryAgent *discoveryAgent;

   // virtual qint64 	readData(char * data, qint64 maxSize);

};

#endif // MAINWINDOW_H
