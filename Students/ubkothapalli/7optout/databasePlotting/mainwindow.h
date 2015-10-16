#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QDebug>



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
    void graphplot(QCustomPlot *customPlot,int i);
    void queryData(QCustomPlot *customPlot, QString dev_name,double x, double y);

    void realtimeDataSlot();
    //void close_qry();
     //  void open_qry();
private:

    Ui::MainWindow *ui;
    QTimer dataTimer;
    double data;
    double timestamp;
    QSqlDatabase accounts_db;
    QString dbaddr;
    QSqlQuery* qry;
    int init_time;
    int checkqry;
    double count_graph;

};

#endif // MAINWINDOW_H
