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

void modelThread::PIDcompute(const double input, double *prevInput, double *output, const double setpoint, double *integral)
{
    double error = setpoint - input;
    double timeInSec = TDELAY/1000;

    /* Summation of integral term */
    *integral += (timeInSec*this->Ki*error);

    /*if (integral > 16)
    {
        integral = 16;
    }*/

    /* PID equation */
    *output = (this->Kp*error) + (*integral) - ((this->Kd*(input-(*prevInput)))/timeInSec);

    /* Setting prev value of error */
    *prevInput = input;
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

double modelThread::coupledFlowRate(double *input, double *input2)
{
    /* Defining the coupled flow rate from defined coupled tube parameters */
    QMutex mutex;
    mutex.lock();
    double coupledHeight = this->coupledHeight;
    double coupledLength = this->coupledLength;
    double coupledRadius = this->coupledRadius;
    double area = this->area;
    mutex.unlock();

    double pressure = 0, pressure2 = 0;

    if (*input > coupledHeight)
    {
        pressure = (*input - coupledHeight) * 9.8 / 100;
    }

    if (*input2 > coupledHeight)
    {
        pressure2 = (*input2 - coupledHeight) * 9.8 / 100;
    }

    double flowRate =
            PI * pow(coupledRadius/100,4) * (pressure - pressure2)
            /
            (8 * 0.00089 * coupledLength/100);

    /* Converting flowrate from cubic meter per sec to cc/10ms*
     * 1 cubic meter/sec = 10000 cc/10ms */
   flowRate *= 10000;

    double flowHeight = flowRate/area;

    /* Flow rate calculated from tank 1 to tank 2. Hence, height reduction in 1 and increase in 2 */
    *input -= flowHeight;
    *input2 += flowHeight;

    return flowRate;
}

void modelThread::run()
{
    QString label;
    QMutex mutex;
    bool mThreadStopLocal = false, objectDropped = false, objVolCalc = false;
    double input, output, setpoint;
    double input2, output2, setpoint2;
    double inFlow = 0.00, outFlow = 0.00, netFlow = 0.00;
    double inFlow2 = 0.00, outFlow2 = 0.00, netFlow2 = 0.00;
    double coupledFlow, coupledHeight, coupledFlowSim;
    double area = this->area;

    static double prevInput = 0, prevInput2 = 0;
    static double integral = 0, integral2 = 0;

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
        input2 = this->input2;
        output2 = this->output2;
        setpoint2 = this->setpoint2;
        coupledFlow = this->coupledFlow;
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
            input2 += this->jump;
            mutex.lock();
            this->objectDropped = false;
            mutex.unlock();
        }

        /* PID compute for this time instant */
        PIDcompute(input, &prevInput, &output, setpoint, &integral);
        PIDcompute(input2, &prevInput2, &output2, setpoint2, &integral2);

        output = int(output);
        output2 = int(output2);

        if (output > 255)
        {
            output = 255;
        }
        else if (output < 0)
        {
            output = 0;
        }

        if (output2 > 255)
        {
            output2 = 255;
        }
        else if (output2 < 0)
        {
            output2 = 0;
        }

        cout << output << "\t" << output2 << "\t" << coupledFlowSim-coupledFlow << endl;

        /* Setting input flow rate from output */
        inFlow = inFlowRate(&input, output);
        inFlow2 = inFlowRate(&input2, output2);

        outFlow = outFlowRate(&input);
        outFlow2 = outFlowRate(&input2);

        coupledFlowSim = coupledFlowRate(&input, &input2);

        /* Calculate coupled flow from change in flow rates and heights of 2 tanks */
        coupledFlow =
                0.5 *
                (
                    (inFlow-inFlow2)
                    -
                    (outFlow-outFlow2)
                    -
                    (
                        (area / TDELAY)
                        *
                        (
                            (input-prevInput)
                            -
                            (input2-prevInput2)
                        )
                    )
                );

        /* calculate height due to coupled flow rate */
        //coupledHeight = coupledFlow/area;

        /* Assuming flow from tank 1 to tank 2, adding the height to tank 1 and subracting from tank 2 */
        //input += coupledHeight;
        //input2 -= coupledHeight;

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

        prevInput = input;
        prevInput2 = input2;

        mutex.lock();
        this->input = input;
        this->input2 = input2;
        this->coupledFlow = coupledFlow;
        mutex.unlock();

        updateSimulation(input, setpoint);

        msleep((unsigned long)TDELAY);
    }

}
