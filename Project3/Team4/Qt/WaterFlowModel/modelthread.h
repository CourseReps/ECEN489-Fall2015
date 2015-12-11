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
    void PIDcompute(const double input, double *prevInput, double *output, const double setpoint, double *integral);
    double inFlowRate(double *input, const double output);
    double outFlowRate(double *input);
    double coupledFlowRate(double *input, double *input2);

    bool mThreadStop;
    bool objectDropped;
    double waterHeight;
    double Kp, Ki, Kd;
    double input, output, setpoint;
    double input2, output2, setpoint2;
    double jump, slope;
    double cmHeight, cmRadius, volume, mmHoleRadius, maxInFlo, area;
    double objectRadius;
    double coupledFlow;
    double coupledHeight;
    double coupledRadius;
    double coupledLength;

signals:
    void setLabel(QString label);
    void updateSimulation(double input, double setpoint);

public slots:
};

#endif // MODELTHREAD_H
