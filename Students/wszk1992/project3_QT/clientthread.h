#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QtCore>
#include <QDebug>
#include <QFile>

class clientThread : public QThread
{
    Q_OBJECT

public:
    explicit clientThread(QObject *parent = 0);
    void run();
    bool clientThreadStop;
    QString serverIP;
signals:
    void setLabelClient(QString label);
};

#endif // CLIENTTHREAD_H

