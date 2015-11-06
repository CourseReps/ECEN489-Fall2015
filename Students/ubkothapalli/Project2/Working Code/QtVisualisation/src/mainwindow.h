#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <qcustomplot.h>
#include "writetodatabase.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addCurve(QCustomPlot *customPlot,int i);
    void addData(QCustomPlot *customPlot, QString dev_name,double x, double y);
    void setgraphs(QCustomPlot *customPlot,QString xlabel,QString ylabel,int rg);

private slots:
    void realtimeDataSlot();
    void sliderValue();


private:
    Ui::MainWindow *ui;
     QTimer dataTimer;
    writetodatabase *database;

signals:

    void sendSetPoint(const QString);
};

#endif // MAINWINDOW_H
