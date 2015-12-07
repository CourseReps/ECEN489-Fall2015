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

    void easyplot();
    bool compute_volumn();
private:
    Ui::MainWindow *ui;
    int max_1;
    int max_2;
    int max_3;
    int check_stable_time_1;
    int check_stable_time_2;
    double stable_err;
    bool stablestate1;
    bool stablestate2;
    bool readytoput;
    bool estimate_state;
    double estimate_volumn;

public slots:
    void onSetLabel(QString label);
    void realtimeDataSlot(double IRRange1, double IRRange2, double PumpRate1, double PumpRate2, double FlowRate);
private slots:
    void on_button_BTstart_clicked();
    void on_button_BTstop_clicked();
    void on_slider_setpoint1_valueChanged(int value);
    void on_pushButton_toggled(bool checked);
};

#endif // MAINWINDOW_H
