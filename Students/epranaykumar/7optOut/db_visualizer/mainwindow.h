#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <qcustomplot.h>

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

private slots:
    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
     QTimer dataTimer;
};

#endif // MAINWINDOW_H
