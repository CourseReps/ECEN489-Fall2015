#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include "modelthread.h"

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

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void onSetLabel(QString label);
    void onUpdateSimulation();

    void on_spinBox_setpoint_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    modelThread *mThread;
};

#endif // MAINWINDOW_H
