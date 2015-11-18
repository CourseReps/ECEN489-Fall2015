#ifndef MODELTHREAD_H
#define MODELTHREAD_H
#include <iostream>
#include <QThread>
#include <QtCore>

#define PI 3.1416

using namespace std;

class modelThread;

class modelThread : public QThread
{
    Q_OBJECT
public:
    explicit modelThread(QObject *parent = 0);
    void run();
    void PIDsetup(double Kp, double Ki, double Kd);
    void PIDcompute(const double input, double *output, const double setpoint);
    void inFlowRate(double *input, const double output);
    void outFlowRate(double *input);

    bool mThreadStop;
    double waterHeight;
    double Kp, Ki, Kd;
    double input, output, setpoint;
    double cmHeight, cmRadius, volume;

signals:
    void setLabel(QString label);
    void updateSimulation();

public slots:
};

#endif // MODELTHREAD_H
