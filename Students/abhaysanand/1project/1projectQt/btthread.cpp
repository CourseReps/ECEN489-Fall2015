#include "btthread.h"
#include "bluetooth_connect.h"

btThread::btThread(QObject *parent) : QThread(parent)
{

}

void btThread::run()
{
    QString dest = "00:12:09:13:99:42";
    QString get_data = "get_data";
    QString label = "Waiting for Bluetooth connection";
    QString btRead;
    QStringList btReadParameters;

    while(1)
    {
        QMutex mutex;

        mutex.lock();

        if(this->btThreadStop)
        {
            break;
        }

        mutex.unlock();

        //btRead = QString(bluetooth_connect(dest, get_data).c_str());
        btRead = bluetooth_connect(dest, get_data);

        if (NULL != btRead)
        {
            label = "Connected to device";
            emit setLabel(label);

            btReadParameters = btRead.split(",", QString::SkipEmptyParts);

            this->IRRange = btReadParameters[0].toDouble();
            this->PumpRate = btReadParameters[1].toDouble();
            this->FlowRate = btReadParameters[2].toDouble();
            this->SolenoidState = btReadParameters[3].toInt();

            emit setParameterValues(IRRange, PumpRate, FlowRate, SolenoidState);
        }
        else
        {
            label = "Connection Failure";
            emit setLabel(label);
        }



        msleep(1000);
    }

    label = "Stopped";

    emit setLabel(label);
}
