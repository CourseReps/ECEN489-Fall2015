#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
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
    void databaseCreator();
    void close_qry();
    void open_qry();
    void on_verticalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QTimer DataTimer;
    double Data;
    double timestamp;
    QSqlDatabase teensy_db;
    QString dbaddr;
    QSqlQuery* qry;
    int init_time;
    int checkqry;

};

#endif // MAINWINDOW_H
