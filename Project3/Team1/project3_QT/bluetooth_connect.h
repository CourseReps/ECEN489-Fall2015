#ifndef BLUETOOTH_CONNECT_H
#define BLUETOOTH_CONNECT_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <QDebug>

#include <QMainWindow>
#include <QFileDialog>
#include <QString>

using namespace std;

/***********************************************************
 * Function: bluetooth_connect
 * Param: dest_addr - MAC address of remote BT device
 *                    of the form "XX:XX:XX:XX:XX:XX"
 *        message - The message to be sent to remote
 *                  BT device
 * Return: returns response from remote BT device 
 *
***********************************************************/
QString bluetooth_connect(QString dest_addr, QString message, bool *conn_state);

void bluetooth_close();

#endif // BLUETOOTH_CONNECT_H
