#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlQueryModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void easyplot();
    void realtimeDataSlot();
private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    QSqlDatabase accounts_db;
    QString dbaddr;
    QSqlQuery* qry1;
    QSqlQuery* qry2;
    QSqlQuery* qry3;
    QSqlQuery* qy;
    QSqlQuery* qry[5];
    int init_time;
    int count11,count21,count22,count31,count32;
    int count12;
    int count[5][2];
    int idcount;
    int i;
    int sum;
    QString IDlist[10];
    double ts[5];
    double devid[5];	
    double maxFlow_rate,maxPump_rate,maxDistance;

};

#endif // MAINWINDOW_H
