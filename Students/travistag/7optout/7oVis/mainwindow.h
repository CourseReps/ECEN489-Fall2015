#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpPlot(QCustomPlot *customPlot);
    void addCurve(QCustomPlot *customPlot);
    static void addToGlobal(int, pair<int, double>);
    static int callback(void *, int, char **, char **);
    static void refreshGlobalDB();


private slots:
    void realtimeDataSlot();



private:
    Ui::MainWindow *ui;
    QTimer dataTimer;

};

#endif // MAINWINDOW_H
