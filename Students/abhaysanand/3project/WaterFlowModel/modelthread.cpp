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

    /*if (integral > 16)
    {
        integral = 16;
    }*/

    /* PID equation */
    *output = (this->Kp*error) + (integral) - ((this->Kd*(input-prevInput))/timeInSec);

    /* Setting prev value of error */
    prevInput = input;
}

double modelThread::inFlowRate(double *input, const double output)
{
    /* Inflow rate linearly proportional to PWM
     * inFlow = slope * PWM + C */
    QMutex mutex;
    mutex.lock();
    double radius = this->cmRadius;
    double slope = this->slope;
    double maxInFlo = this->maxInFlo;
    mutex.unlock();

    /* inFlow -> 1 lit/min -> 1000cc/min -> (1000/60)cc/sec -> (1/6)cc/10ms
     * Divided by 6 to convert to cc per 10 ms */
    double inFlow = (slope * output) / 6;

    if (inFlow > maxInFlo/6)
    {
        inFlow = maxInFlo/6;
    }

    double inHeight = inFlow/(PI * radius * radius);

    *input += inHeight;

    return inFlow;
}

double modelThread::outFlowRate(double *input)
{
    /* outflow rate is proportional to root of height of water
     * outFlow = area * sqrt(2*g*H) */
    QMutex mutex;
    mutex.lock();
    double radius = this->cmRadius;
    double cmHoleRadius = this->mmHoleRadius / 10;
    mutex.unlock();

    double area = PI * cmHoleRadius * cmHoleRadius;

    /* Divided by 6 to convert to cc per 10 ms */
    double outFlow = area * sqrt(2 * 980 * (*input) / 10000);

    double outHeight = outFlow/(PI * radius * radius);

    *input -= outHeight;

    return outFlow;
}

void modelThread::run()
{
    QString label;
    QMutex mutex;
    bool mThreadStopLocal = false, objectDropped = false, objVolCalc = false;
    double input, output, setpoint;
    double inFlow = 0.00, outFlow = 0.00, netFlow = 0.00;

    /* Setup PID coefficients */
    PIDsetup(30,2,2);

    label = "Simulation Started";
    emit setLabel(label);

    while(1)
    {
        mutex.lock();
        mThreadStopLocal = this->mThreadStop;
        objectDropped = this->objectDropped;
        input = this->input;
        output = this->output;
        setpoint = this->setpoint;
        mutex.unlock();

        if (mThreadStopLocal)
        {
            label = "Simulation Stopped";
            emit setLabel(label);

            break;
        }
        if (objectDropped == true)
        {
            objVolCalc = true;
            input += this->jump;
            mutex.lock();
            this->objectDropped = false;
            mutex.unlock();
        }

        /* PID compute for this time instant */
        PIDcompute(input, &output, setpoint);

        output = int(output);

        if (output > 255)
        {
            output = 255;
        }
        else if (output < 0)
        {
            output = 0;
        }
        cout << output << endl;

        /* Setting input flow rate from output */
        inFlow = inFlowRate(&input, output);
        outFlow = outFlowRate(&input);

        /* Once object is dropped, calculating volume with in and out flow rates */
        if (objVolCalc == true)
        {
            netFlow += (outFlow - inFlow);

            if ((input >= setpoint-0.01) && (input <= setpoint+0.01))
            {
                objVolCalc = false;
                label = "Object Volume = " + QString::number(netFlow) + " cc";
                emit setLabel(label);
                netFlow = 0.00;
            }
        }

        mutex.lock();
        this->input = input;
        mutex.unlock();

        updateSimulation(input, setpoint);

        msleep((unsigned long)TDELAY);
    }

}
