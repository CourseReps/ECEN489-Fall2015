#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include "macro_value.h"
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
    bool check_stablestate(int flag);
    void compute_volumn();
public slots:
    void realtimeDataSlot();
    void value1(int);
    void value2(int);

    void value_d1(int value);
    void value_d2(int value);
    void value_d3(int value);
    void put_item(int volumn);
    void change_volumn(int volumn);
private slots:
    void on_pushButton_clicked();

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
    int check_stable_time_1;
    int check_stable_time_2;
    bool stablestate_1;
    bool stablestate_2;
    int item_volumn;
    bool start_estimate_volumn;
    double estimated_volumn;
    QVector<QCustomPlot*> plotvector;
};


#endif // MAINWINDOW_H
