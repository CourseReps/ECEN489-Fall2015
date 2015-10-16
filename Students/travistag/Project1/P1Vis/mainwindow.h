#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include <map>
#include <vector>
#include <stdlib.h>
#include <string>

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
    void addCurve(QCustomPlot *customPlot, int pn);
    static void addToGlobal(string, pair<int, vector<double> >);
    static int callback(void *, int, char **, char **);
    void refreshGlobalDB();
    void setDBLoc(string);
    string getDBLoc();


private slots:
    void realtimeDataSlot();



private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    string dbloc="";

};

#endif // MAINWINDOW_H
