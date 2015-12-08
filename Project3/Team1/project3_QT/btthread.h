#ifndef BTTHREAD_H
#define BTTHREAD_H

#include <QThread>
#include <QtCore>
#include <QDebug>

class btThread : public QThread
{
    Q_OBJECT

public:
    explicit btThread(QObject *parent = 0);
    void run();
    bool btThreadStop;
    double IRSetpoint1;
    double IRSetpoint2;
    double IRRange1;
    double IRRange2;
    double PumpRate1;
    double PumpRate2;
    double FlowRate;
    double prevsetpoint1;
    double prevsetpoint2;
    double estimate_volumn;


signals:
    void setLabel(QString label);
    void setParameterValues(double IRRange1, double IRRange2, double PumpRate1, double PumpRate2, double FlowRate, double estimate_volumn);

public slots:
};

#endif // BTTHREAD_H
