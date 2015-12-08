#include "btthread.h"
#include "bluetooth_connect.h"

btThread::btThread(QObject *parent) : QThread(parent)
{
    IRSetpoint1 = 2;
    IRSetpoint2 = 2;
    prevsetpoint1 = 0;
    prevsetpoint2 = 0;
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


    while(1)
    {
        mutex.lock();
        btThreadStopLocal = this->btThreadStop;
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

            if (prevsetpoint1 != IRSetpoint1 || prevsetpoint2 != IRSetpoint2)
            {
                send_data = "send_data1 "+ QString::number(IRSetpoint1,'f', 2) ;

                qDebug() << send_data;

                btRead = bluetooth_connect(dest, send_data, &conn_state);

                /* If the data is sent and acknowledge correctly received, then
             * set previous value of IR variable to current value */
                if ((NULL != btRead) && (true == btRead.toInt()))
                {
                    prevsetpoint1 = IRSetpoint1;
                    prevsetpoint2 = IRSetpoint2;
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
        qDebug()<< btRead<<endl;
        if (NULL != btRead)
        {
            QStringList btReadParameters;
            label = "Connected to device";
            emit setLabel(label);
            btReadParameters = btRead.split(",", QString::SkipEmptyParts);
            qDebug()<< btReadParameters[0].toDouble()<<endl;
            this->IRRange1 = btReadParameters[0].toDouble();
            this->IRRange2 = btReadParameters[1].toDouble();
            this->PumpRate1 = btReadParameters[2].toDouble();
            this->PumpRate2 = btReadParameters[3].toDouble();
            this->FlowRate = btReadParameters[4].toDouble();
            this->estimate_volumn = btReadParameters[5].toDouble();

            emit setParameterValues(IRRange1, IRRange2, PumpRate1, PumpRate2, FlowRate, estimate_volumn);
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
