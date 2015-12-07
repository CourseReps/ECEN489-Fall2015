#ifndef CLIENT_CONNECT_H
#define CLIENT_CONNECT_H

#include <QtCore>
#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

QString client_connect(QString serverIP, bool *conn_state);
void client_close();

#endif // CLIENT_CONNECT_H

