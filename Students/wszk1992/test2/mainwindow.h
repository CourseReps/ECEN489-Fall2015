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

#define MAXCLIENT 3

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
    double data;
    double timestamp;
    QSqlDatabase accounts_db;
    QString dbaddr;
    QSqlQuery *qry;
    QString deviceID;
    QString IDlist[MAXCLIENT];
    int clientnum;
    int clientsum;
    int limit;
    int select;
    double value[MAXCLIENT];


};

#endif // MAINWINDOW_H
