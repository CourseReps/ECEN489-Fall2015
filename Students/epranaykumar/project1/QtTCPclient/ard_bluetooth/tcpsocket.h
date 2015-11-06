#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include "json/json.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileInfo>
#include <QtCore>


#include  <unistd.h>
#include <time.h>

class TCPSocket : public QObject
{
    Q_OBJECT
public:
  //  explicit TCPSocket(QObject *parent = 0);
    //~TCPSocket;
   explicit  TCPSocket();


    void stopTCPSocket();
    void sendDatabase(boost:: asio::ip::tcp::socket& socket);

signals:
    // void gotpollingReq();
     void finished();

public slots:
      void ConnecttoTCPserver();



};

#endif // TCPSOCKET_H
