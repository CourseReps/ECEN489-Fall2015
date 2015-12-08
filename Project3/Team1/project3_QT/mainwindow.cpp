#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mThread = new btThread(this);
    cThread = new clientThread(this);

    mThread->btThreadStop = true;
    cThread->clientThreadStop = true;

    easyplot();
    max_1 = 15;
    max_2 = 100;
    max_3 = 8;
    stable_err = 1;

    check_stable_time_1 = 0;
    check_stable_time_2 = 0;
    stablestate1 = 0;
    stablestate2 = 0;
    estimate_volumn = 0;


    ui->setpoint1->setValue(mThread->IRSetpoint1);
    ui->setpoint1->setRange(0,10);
    ui->setpoint2->setValue(mThread->IRSetpoint2);
    ui->setpoint2->setRange(0,10);
    ui->spinBox_1->setMaximum(max_1);
    ui->spinBox_1->setMinimum(0);
    ui->spinBox_1->setValue(mThread->IRSetpoint1);
    ui->spinBox_2->setMaximum(max_1);
    ui->spinBox_2->setMinimum(0);
    ui->spinBox_2->setValue(mThread->IRSetpoint2);

    connect(mThread, SIGNAL(setLabel(QString)), this, SLOT(onSetLabel(QString)));
    connect(mThread, SIGNAL(setParameterValues(double,double,double,double,double,double)), this, SLOT(realtimeDataSlot(double,double,double,double,double,double)));
    connect(ui->spinBox_1,SIGNAL(valueChanged(int)),ui->setpoint1,SLOT(setValue(int)));
    connect(ui->setpoint1,SIGNAL(valueChanged(int)),ui->spinBox_1,SLOT(setValue(int)));
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),ui->setpoint2,SLOT(setValue(int)));
    connect(ui->setpoint2,SIGNAL(valueChanged(int)),ui->spinBox_2,SLOT(setValue(int)));


    ui->statusBar->showMessage("Waiting for BT connection...", 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::easyplot()
{
    //IRSensor
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, "IR Sensor"));
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(1)->setAntialiasedFill(false);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->yAxis->setRange(0, max_1);
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Distance (cm)");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));




    //Pump
    ui->customPlot_2->addGraph(); // blue line
    ui->customPlot_2->plotLayout()->insertRow(0);
    ui->customPlot_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot_2, "Pump Rate"));
    ui->customPlot_2->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot_2->graph(0)->setAntialiasedFill(false);


    ui->customPlot_2->addGraph(); // red line
    ui->customPlot_2->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot_2->graph(1)->setAntialiasedFill(false);

    ui->customPlot_2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_2->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_2->xAxis->setAutoTickStep(false);
    ui->customPlot_2->xAxis->setTickStep(2);
    ui->customPlot_2->axisRect()->setupFullAxesBox();

    ui->customPlot_2->yAxis->setRange(0, max_2);
    ui->customPlot_2->xAxis->setLabel("Time");
    ui->customPlot_2->yAxis->setLabel("Liters per minute");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_2->yAxis2, SLOT(setRange(QCPRange)));

    //Flow Rate
    ui->customPlot_3->addGraph(); // blue line
    ui->customPlot_3->plotLayout()->insertRow(0);
    ui->customPlot_3->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot_3, "Flow Sensor Rate"));
    ui->customPlot_3->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot_3->graph(0)->setAntialiasedFill(false);


    ui->customPlot_3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_3->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_3->xAxis->setAutoTickStep(false);
    ui->customPlot_3->xAxis->setTickStep(2);
    ui->customPlot_3->axisRect()->setupFullAxesBox();

    ui->customPlot_3->yAxis->setRange(0, max_3);
    ui->customPlot_3->xAxis->setLabel("Time");
    ui->customPlot_3->yAxis->setLabel("Liters per minute");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_3->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_3->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::realtimeDataSlot(double IRRange1, double IRRange2, double PumpRate1, double PumpRate2, double FlowRate, double estimate_volumn)
{
    double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    // add IRRange1 to lines:
    ui->customPlot->graph(0)->addData(key, IRRange1);

    // remove IRRange that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);

    // add IRRange2 to lines:
    ui->customPlot->graph(1)->addData(key, IRRange2);

    // remove IRRange that's outside visible range:
    ui->customPlot->graph(1)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot->yAxis->setRange(0, max_1);
    ui->customPlot->replot();


    // add pumpRate1 to lines:
    ui->customPlot_2->graph(0)->addData(key, PumpRate1);

    // remove pumpRate1 data that's outside visible range:
    ui->customPlot_2->graph(0)->removeDataBefore(key-15);

    // add pumpRate2 to lines:
    ui->customPlot_2->graph(1)->addData(key, PumpRate2);

    // remove pumpRate2 data that's outside visible range:
    ui->customPlot_2->graph(1)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_2->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_2->yAxis->setRange(0, max_2);
    ui->customPlot_2->replot();


    // add PumpRate to lines:
    ui->customPlot_3->graph(0)->addData(key, FlowRate);

    // remove PumpRate data that's outside visible range:
    ui->customPlot_3->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_3->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_3->yAxis->setRange(0, max_3);
    ui->customPlot_3->replot();

    if(check_stablestate(1))
    {
        qDebug()<<"1 stable"<<endl;
        ui->textBrowser_1->setText("stable");
        stablestate1 = 1;
    }
    else
    {
        ui->textBrowser_1->setText("unstable");
        stablestate1 = 0;
    }
    if(check_stablestate(2))
    {
        qDebug()<<"2 stable"<<endl;
        ui->textBrowser_2->setText("stable");
        stablestate2 = 1;
    }
    else
    {
        ui->textBrowser_2->setText("unstable");
        stablestate2 = 0;
    }
    if(estimate_volumn>0)
    {
         qDebug()<< "get volume"<<endl;
        ui->textBrowser->setText(QString::number(estimate_volumn));
    }

//    if(readytoput)
//    {
//        if(!stablestate1)
//        {
//            estimate_state = true;
//            //TODO:
//           // compute_volumn();
//            ui->textBrowser->setText(QString::number(estimate_volumn));
//        }
//        else if (estimate_state)
//        {
//            estimate_state = false;
//            qDebug()<<"Estimated over!"<<endl;
//            //NOTICE:
//            ui->pushButton->toggled(false);
//        }
//    }

}

bool MainWindow::check_stablestate(int flag)
{
    int* ptime;
    double h;
    double setpoint;
    if(flag==1)
    {
       ptime = &check_stable_time_1;
       h = mThread->IRRange1;
       setpoint=mThread->IRSetpoint1;
    }
    else if(flag==2)
    {
        ptime = &check_stable_time_2;
        h = mThread->IRRange2;
        setpoint=mThread->IRSetpoint2;
    }

    if(abs(h-setpoint)<stable_err)
    {
        (*ptime)++;
        if(*ptime<100)
            return false;
        else
            return true;
    }
    else
    {
        *ptime=0;
        return false;
    }
}

//TODO:
bool MainWindow::compute_volumn()
{

}

void MainWindow::onSetLabel(QString label)
{
    ui->statusBar->showMessage(label,0);
}

void MainWindow::on_button_BTstart_clicked()
{
    /* If stop thread flag is true, only then thread will be started. */
    if(true == mThread->btThreadStop)
    {
        mThread->btThreadStop = false;
        mThread->IRSetpoint1 = 2;
        mThread->IRSetpoint2 = 2;
        mThread->start();
    }
}

void MainWindow::on_button_BTstop_clicked()
{
    /* Set threadStop flag to true */
    mThread->btThreadStop = true;
}

void MainWindow::on_setpoint1_valueChanged(int value)
{
    //NOTICE: need to know the unit
    mThread->IRSetpoint1 = value;
}

void MainWindow::on_setpoint2_valueChanged(int value)
{
    //NOTICE: need to know the unit
    mThread->IRSetpoint2 = value;
}




