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
    bool connectToClient(int i);
    void establishConnection();
    void sigchld_handler(int s);
    char* hostIPAddr = new char;
    bool btThreadStop;
    double IRRange;
    double FlowRate;
    double PumpRate;
    bool SolenoidState;
    QString ID;
    QString CurrentIP;
    int Timestamp;
    int mainsocket;
    int sockfd[5];

signals:
    void setParameterValues(double IRRange, double PumpRate, double FlowRate, bool SolenoidState, QString ID, int Timestamp, QString CurrentIP);

public slots:
};

#endif // BTTHREAD_H
