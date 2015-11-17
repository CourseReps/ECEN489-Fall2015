#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <pid_controller.h>
#include <watersystem.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupRealtimeDataDemo(QVector<QCustomPlot *> plotvector);
    void addData(QCustomPlot *customPlot, QVector<double> value, double key);
    void addCurve(QCustomPlot *customPlot, double upper_level, double range, double key);
public slots:
    void realtimeDataSlot();
    void value(int);

private:
    Ui::MainWindow *ui;
    Watersystem *system;
    PIDController *controller1;
    PIDController *controller2;
    QString demoName;
    QTimer dataTimer;
    double setpoint1;
    double setpoint2;
    int timer;
    QVector<QCustomPlot*> plotvector;
};


#endif // MAINWINDOW_H
