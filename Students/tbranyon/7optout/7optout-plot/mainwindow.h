#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <unistd.h>
#include "sqlite3.h"
#include <iostream>
#include "qcustomplot.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QVector<QString> entryKeys;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    long starttime;
    QVector<QVector<double> > times;
    QVector<QVector<double> > ADCvals;

private slots:
    void readData();
    void setupGraph(QCustomPlot *graphplot);
    static int readKeys(void *na, int argc, char **argv, char **colname);
};

#endif // MAINWINDOW_H
