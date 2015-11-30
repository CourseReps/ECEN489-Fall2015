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
    double inFlowRate(double *input, const double output);
    double outFlowRate(double *input);

    bool mThreadStop;
    bool objectDropped;
    double waterHeight;
    double Kp, Ki, Kd;
    double input, output, setpoint, jump, slope;
    double cmHeight, cmRadius, volume, mmHoleRadius, maxInFlo;
    double outFlow;
    double objectRadius;

signals:
    void setLabel(QString label);
    void updateSimulation(double input, double setpoint);

public slots:
};

#endif // MODELTHREAD_H
