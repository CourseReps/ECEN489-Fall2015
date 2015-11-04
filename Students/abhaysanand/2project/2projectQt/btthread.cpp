#include "btthread.h"
#include "bluetooth_connect.h"

btThread::btThread(QObject *parent) : QThread(parent)
{

}

void btThread::run()
{
    QMutex mutex;
    QString dest = "00:12:09:13:99:42";
    QString get_data = "get_data";
    QString send_data;
    QString label;
    QString btRead;
    static bool conn_state = false;
    bool btThreadStopLocal;
    double setpoint = 2;
    double prevSetpoint = 2;

    while(1)
    {
        mutex.lock();
        btThreadStopLocal = this->btThreadStop;
        setpoint = this->IRSetpoint;
        mutex.unlock();

        if(btThreadStopLocal)
        {
            bluetooth_close();
            conn_state = false;

            label = "BT disconnected";
            emit setLabel(label);

            break;
        }

        /* Check if there is change in slider value.
         * If change, then send new value to teensy */
        if (prevSetpoint != setpoint)
        {
            send_data = "send_data"+ QString::number(setpoint,'f', 2);
            cout << send_data.toStdString();

            btRead = bluetooth_connect(dest, send_data, &conn_state);

            /* If the data is sent and acknowledge correctly received, then
             * set previous value of IR variable to current value */
            if ((NULL != btRead) && (true == btRead.toInt()))
            {
                prevSetpoint = setpoint;
            }
            else
            {
                label = "Connection Failure: send_data ack not received...";
                emit setLabel(label);
            }
        }

        msleep(100);

        /* Get current IR value from teensy */
        btRead = bluetooth_connect(dest, get_data, &conn_state);

        if (NULL != btRead)
        {
            label = "Connected to BT device";
            emit setLabel(label);

            cout << btRead.toDouble();

            emit setParameterValues(btRead.toDouble());
        }
        else
        {
            label = "Connection Failure: get_data ack not received...";
            emit setLabel(label);
        }

        msleep(100);
    }

    label = "BT Stopped";
    emit setLabel(label);
}
