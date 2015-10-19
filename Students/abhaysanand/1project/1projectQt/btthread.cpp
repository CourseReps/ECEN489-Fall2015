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
    emit setLabel(label);
    QString btRead;
    QStringList btReadParameters;
    static bool conn_state = false;

    while(1)
    {
        QMutex mutex;
        bool btThreadStopLocal;

        mutex.lock();
        btThreadStopLocal = this->btThreadStop;
        mutex.unlock();

        if(btThreadStopLocal)
        {
            bluetooth_close();
            conn_state = false;

            label = "Stopped";
            emit setLabel(label);

            break;
        }

        btRead = bluetooth_connect(dest, get_data, &conn_state);

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

        msleep(200);
    }

    label = "Stopped";
    emit setLabel(label);
}
