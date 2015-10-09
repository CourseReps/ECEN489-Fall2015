#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <unistd.h>
#include "sqlite3.h"
#include <iostream>
#include "qcustomplot.h"
#include <QTimer>
#include <cstdlib>
#include <cstdio>
#include <ctime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QVector<QString> entryKeys;
    QVector<double> times[8];
    QVector<double> ADCvals[8];
    sqlite3 *db;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    time_t starttime;

private slots:
    void readData();
    void setupGraph(QCustomPlot *graphplot);
    static int readKeys(void *na, int argc, char **argv, char **colname);
    static int readVals(void *na, int argc, char **argv, char **colname);
};

#endif // MAINWINDOW_H
