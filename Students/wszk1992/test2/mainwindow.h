#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QtSql/qsqlquery.h>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>



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
    void init_sql();
    void get_query_sql(QSqlQuery *query);
    void easyplot();
    void realtimeDataSlot();
    void get_data_sql(QSqlQuery *query);
    void get_timestamp_sql(QSqlQuery *query);
private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    double data;
    double timestamp;
    QSqlDatabase accounts_db;
};

#endif // MAINWINDOW_H
