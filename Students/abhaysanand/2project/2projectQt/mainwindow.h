#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtSql>
#include <QFileInfo>
#include <string>
#include "btthread.h"
#include "clientthread.h"
#include <QBluetoothAddress>
#include <QBluetoothSocket>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlQueryModel>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    btThread *mThread;
    clientThread *cThread;

private:
    Ui::MainWindow *ui;

public slots:
    void onSetLabel(QString label);
    void onSetParameterValues(double IRCurrent);
private slots:
    void on_button_BTstart_clicked();
    void on_button_BTstop_clicked();
    void on_slider_setpoint_valueChanged(int value);
};

#endif // MAINWINDOW_H
