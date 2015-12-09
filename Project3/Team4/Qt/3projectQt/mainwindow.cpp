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

    connect(mThread, SIGNAL(setLabel(QString)), this, SLOT(onSetLabel(QString)));
    connect(mThread, SIGNAL(setParameterValues(QString)), this, SLOT(onSetParameterValues(QString)));

    ui->statusBar->showMessage("Waiting for BT connection...", 0);

    plotInit();
}

MainWindow::~MainWindow()
{
    delete ui;
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
        mThread->IRSetpoint = 2;
        mThread->start();
    }
}

void MainWindow::on_button_BTstop_clicked()
{
    /* Set threadStop flag to true */
    mThread->btThreadStop = true;
}

void MainWindow::on_slider_setpoint_valueChanged(int value)
{
    double setpoint = 2;
    setpoint = ((double)value / 2) + 2;

    mThread->IRSetpoint = setpoint;

    /* Set slider value to spinBox */
    ui->spin_setpoint->setValue(setpoint);
}

void MainWindow::onSetParameterValues(QString btRead)
{
    /* Decode the message sent from Teensy - comma separated values */
    QStringList btList = btRead.split(',');
    double height = 0, inFlowRate = 0, outFlowRate = 0;

    if(btList.size() == 4)
    {
        height = btList[0].toDouble();
        inFlowRate = btList[1].toDouble();
        outFlowRate = btList[2].toDouble();
        //objVolume = btList[3].toDouble();
    }
    else
    {
        qDebug() << "Data receive error!!!\n";
    }

    /* Set current value to slider and spinBox */
    int current = int((height - 2) * 10);
    ui->slider_current->setValue(current);
    ui->spin_current->setValue(height);

    double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    // add inflowrate to plot
    ui->plot_inFlow->graph(0)->addData(key, inFlowRate);
    // remove value that's outside visible range:
    ui->plot_inFlow->graph(0)->removeDataBefore(key-15);

    // add outflowrate to plot
    ui->plot_outFlow->graph(0)->addData(key, outFlowRate);
    // remove value that's outside visible range:
    ui->plot_outFlow->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->plot_inFlow->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->plot_inFlow->yAxis->setRange(0, 3);
    ui->plot_inFlow->replot();

    ui->plot_outFlow->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->plot_outFlow->yAxis->setRange(0, 3);
    ui->plot_outFlow->replot();
}

void MainWindow::plotInit()
{
    /* Get the background color of the whole widget */
    QColor bgColor = ui->widget->palette().color(QWidget::backgroundRole());

    /* Inflow rate plot */
    ui->plot_inFlow->setBackground(bgColor);    // Set widget BG color to BG of customplot widget
    ui->plot_inFlow->axisRect()->setBackground(Qt::white);  // Set the BG of the axisRectangle to white
    ui->plot_inFlow->addGraph(); // blue line
    ui->plot_inFlow->plotLayout()->insertRow(0);
    ui->plot_inFlow->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->plot_inFlow, "In-Flow Rate"));
    ui->plot_inFlow->graph(0)->setPen(QPen(Qt::blue));
    ui->plot_inFlow->graph(0)->setAntialiasedFill(false);

    ui->plot_inFlow->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot_inFlow->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->plot_inFlow->xAxis->setAutoTickStep(false);
    ui->plot_inFlow->xAxis->setTickStep(2);
    ui->plot_inFlow->axisRect()->setupFullAxesBox();

    ui->plot_inFlow->yAxis->setRange(0, 3);
    ui->plot_inFlow->xAxis->setLabel("Time");
    ui->plot_inFlow->yAxis->setLabel("Flow Rate (L/min)");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_inFlow->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_inFlow->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_inFlow->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_inFlow->yAxis2, SLOT(setRange(QCPRange)));

    //Outflow rate plot
    ui->plot_outFlow->setBackground(bgColor);
    ui->plot_outFlow->axisRect()->setBackground(Qt::white);
    ui->plot_outFlow->addGraph(); // blue line
    ui->plot_outFlow->plotLayout()->insertRow(0);
    ui->plot_outFlow->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->plot_outFlow, "Out-Flow Rate"));
    ui->plot_outFlow->graph(0)->setPen(QPen(Qt::blue));
    ui->plot_outFlow->graph(0)->setAntialiasedFill(false);

    ui->plot_outFlow->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot_outFlow->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->plot_outFlow->xAxis->setAutoTickStep(false);
    ui->plot_outFlow->xAxis->setTickStep(2);
    ui->plot_outFlow->axisRect()->setupFullAxesBox();

    ui->plot_outFlow->yAxis->setRange(0, 3);
    ui->plot_outFlow->xAxis->setLabel("Time");
    ui->plot_outFlow->yAxis->setLabel("Flow Rate (L/min)");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_outFlow->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_outFlow->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_outFlow->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_outFlow->yAxis2, SLOT(setRange(QCPRange)));
}
