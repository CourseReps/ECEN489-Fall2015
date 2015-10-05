#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qcustomplot.h"
#include <QMainWindow>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    void easyplot();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void realtimeDataSlot();
    QTimer dataTimer;
};

#endif // MAINWINDOW_H
