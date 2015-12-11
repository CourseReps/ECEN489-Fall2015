#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include "modelthread.h"
#include <QtMath>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void plotInit();

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void onSetLabel(QString label);
    void onUpdateSimulation(double input, double setpoint);

    void on_spinBox_setpoint_valueChanged(double arg1);

    void on_pushButton_drop_clicked();

    void on_spinBox_setpoint2_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    modelThread *mThread;
};

#endif // MAINWINDOW_H
