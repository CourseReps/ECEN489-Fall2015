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
    double IRRange;
    double FlowRate;
    double PumpRate;
    bool SolenoidState;

signals:
    void setLabel(QString label);
    void setParameterValues(double IRRange, double PumpRate, double FlowRate, bool SolenoidState);

public slots:
};

#endif // BTTHREAD_H
