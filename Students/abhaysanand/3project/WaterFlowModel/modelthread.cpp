#include "modelthread.h"
/* Time interval in ms */
#define TDELAY 10.00

modelThread::modelThread(QObject *parent) : QThread(parent)
{

}

void modelThread::PIDsetup(double Kp, double Ki, double Kd)
{
    QMutex mutex;

    mutex.lock();
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    mutex.unlock();
}

void modelThread::PIDcompute(const double input, double *output, const double setpoint)
{
    double error = setpoint - input;
    static double prevInput = 0;
    static double integral = 0;
    double timeInSec = TDELAY/1000;

    /* Summation of integral term */
    integral += (timeInSec*this->Ki*error);

    if (integral > 16)
    {
        integral = 16;
    }

    /* PID equation */
    *output = (this->Kp*error) + (integral) - ((this->Kd*(input-prevInput))/timeInSec);

    /* Setting prev value of error */
    prevInput = input;
}

void modelThread::inFlowRate(double *input, const double output)
{
    /* 18 liters/min = 18000cc/min = 300cc/sec = 3cc/10ms */
    QMutex mutex;
    mutex.lock();
    double radius = this->cmRadius;
    mutex.unlock();

    double currHeight = 3/(PI * radius * radius);

    double outHeight = output * currHeight;

    if (outHeight > currHeight)
    {
        outHeight = currHeight;
    }

    *input += outHeight;
}

void modelThread::outFlowRate(double *input)
{
    /* with full height, assume 18 liters/min = 3 cc/10ms */
    QMutex mutex;
    mutex.lock();
    double radius = this->cmRadius;
    mutex.unlock();

    double currHeight = *input * (3/(PI * radius * radius)) / 16;

    *input -= currHeight;
}

void modelThread::run()
{
    QString label;
    QMutex mutex;
    bool mThreadStopLocal;
    double input, output, setpoint;

    /* Setup PID coefficients */
    PIDsetup(4,1.4,1.5);

    label = "Simulation Started";
    emit setLabel(label);

    while(1)
    {
        mutex.lock();
        mThreadStopLocal = this->mThreadStop;
        input = this->input;
        output = this->output;
        setpoint = this->setpoint;
        mutex.unlock();

        if(mThreadStopLocal)
        {
            label = "Simulation Stopped";
            emit setLabel(label);

            break;
        }

        /* PID compute for this time instant */
        PIDcompute(input, &output, setpoint);

        cout << output << endl;

        /*if (output > 255)
        {
            output = 255.00;
        }
        else if (output < -255)
        {
            output = -255.00;
        }*/

        /* Setting input flow rate from output */
        inFlowRate(&input, output);
        outFlowRate(&input);

        mutex.lock();
        this->input = input;
        mutex.unlock();

        updateSimulation();

        msleep((unsigned long)TDELAY);
    }

}
