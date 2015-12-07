#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mThread = new modelThread(this);

    connect(mThread, SIGNAL(setLabel(QString)), this, SLOT(onSetLabel(QString)));
    connect(mThread, SIGNAL(updateSimulation(double,double)), this, SLOT(onUpdateSimulation(double,double)));

    QFile file("../WaterFlowModel/inputFile.txt");
    QStringList values;

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            values += in.readLine().split(",");
        }
    }
    else
    {
        ui->statusBar->showMessage("Error opening inputFile.txt");
    }

    mThread->cmRadius = ((QString(values[0])).toDouble()) / 2;
    mThread->cmHeight = (QString(values[1])).toDouble();
    mThread->mmHoleRadius = ((QString(values[2])).toDouble() / 2);
    mThread->maxInFlo = (QString(values[3])).toDouble();
    mThread->setpoint = (QString(values[4])).toDouble();
    mThread->jump = (QString(values[5])).toDouble();

    mThread->setpoint2 = mThread->setpoint;

    mThread->area = PI * mThread->cmRadius * mThread->cmRadius;
    mThread->volume = mThread->area * mThread->cmHeight;
    mThread->slope = mThread->maxInFlo/255;

    mThread->coupledHeight = (double)2;
    mThread->coupledRadius = (double)0.5;
    mThread->coupledLength = (double)3;

    ui->spinBox_setpoint->setValue(mThread->setpoint);
    ui->spinBox_setpoint2->setValue(mThread->setpoint2);
    mThread->mThreadStop = true;
    mThread->objectDropped = false;
    mThread->input = 0;
    mThread->output = 0;
    mThread->input2 = 0;
    mThread->output2 = 0;
    mThread->objectRadius = 0;
    mThread->coupledFlow = 0;

    plotInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    if (true == mThread->mThreadStop)
    {
        mThread->mThreadStop = false;
        mThread->start();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    double setpoint = mThread->setpoint;
    double setpoint2 = mThread->setpoint2;
    double current = mThread->input;
    double current2 = mThread->input2;

    double cmHeight = mThread->cmHeight;
    double cmRadius = mThread->cmRadius;

    /* Origin of QWidget */
    int x = ui->widget->x();
    int y = ui->widget->y();
    /* Origin of QWidget2 */
    int x2 = ui->widget_2->x();
    int y2 = ui->widget_2->y();
    /* Dimensions of QWidget */
    int width = ui->widget->width();
    int height = ui->widget->height();
    /* Dimensions of QWidget2 */
    int width2 = ui->widget_2->width();
    int height2 = ui->widget_2->height();
    /* Defining tank dimensions. Keeping tank width as constant QWidget */
    int tankWidth = width/2;
    int tankHeight = (cmHeight * tankWidth) /(cmRadius*2);
    int originX = x + width/2;
    int originY = y + height/2;
    int objectRadius = mThread->objectRadius * tankHeight /cmHeight;
    /* Defining tank dimensions. Keeping tank width as constant QWidget2 */
    int tankWidth2 = width2/2;
    int tankHeight2 = (cmHeight * tankWidth2) /(cmRadius*2);
    int originX2 = x2 + width2/2;
    int originY2 = y2 + height2/2;
    int objectRadius2 = mThread->objectRadius * tankHeight2 /cmHeight;

    if (setpoint < 2)
    {
        setpoint = 2;
    }
    else if (setpoint > 15)
    {
        setpoint = 15;
    }

    if (setpoint2 < 2)
    {
        setpoint2 = 2;
    }
    else if (setpoint2 > 15)
    {
        setpoint2 = 15;
    }

    setpoint = (setpoint * tankHeight) / cmHeight;
    current = (current * tankHeight) / cmHeight;
    setpoint2 = (setpoint2 * tankHeight2) / cmHeight;
    current2 = (current2 * tankHeight2) / cmHeight;
    //ui->statusBar->showMessage(QString::tos(setpoint));

    QPainter p;
    p.begin(this);
    p.drawRect(x,y,x2-x+width2,height);
    //p.drawRect(x2,y2,width2,height2);
    /* Left line */
    p.drawLine(originX-(tankWidth/2), originY-(tankHeight/2), originX-(tankWidth/2), originY+(tankHeight/2));
    p.drawLine(originX2-(tankWidth2/2), originY2-(tankHeight2/2), originX2-(tankWidth2/2), originY2+(tankHeight2/2));
    /* Right line */
    p.drawLine(originX+(tankWidth/2), originY-(tankHeight/2), originX+(tankWidth/2), originY+(tankHeight/2));
    p.drawLine(originX2+(tankWidth2/2), originY2-(tankHeight2/2), originX2+(tankWidth2/2), originY2+(tankHeight2/2));
    /* Bottom line */
    p.drawLine(originX-(tankWidth/2), originY+(tankHeight/2), originX+(tankWidth/2), originY+(tankHeight/2));
    p.drawLine(originX2-(tankWidth2/2), originY2+(tankHeight2/2), originX2+(tankWidth2/2), originY2+(tankHeight2/2));
    /* Setpoint line */
    Qt::GlobalColor setpointColor;
    if((current >= (setpoint-3)) && (current <= (setpoint+3)))
    {
        setpointColor = Qt::green;
    }
    else if((current >= (setpoint-10)) && (current <= (setpoint+10)))
    {
        setpointColor = Qt::darkYellow;
    }
    else
    {
        setpointColor = Qt::red;
    }
    p.fillRect(originX-(tankWidth/2), originY+(tankHeight/2)-(int)setpoint-1, -15, 3, setpointColor);

    if((current2 >= (setpoint2-3)) && (current2 <= (setpoint2+3)))
    {
        setpointColor = Qt::green;
    }
    else if((current2 >= (setpoint2-10)) && (current2 <= (setpoint2+10)))
    {
        setpointColor = Qt::darkYellow;
    }
    else
    {
        setpointColor = Qt::red;
    }
    p.fillRect(originX2+(tankWidth2/2)+1, originY2+(tankHeight2/2)-(int)setpoint2-1, 15, 3, setpointColor);

    /* Water height */
    p.fillRect(1+originX-(tankWidth/2), originY+(tankHeight/2), tankWidth-2, (int)-current, Qt::blue);
    p.fillRect(1+originX2-(tankWidth2/2), originY2+(tankHeight2/2), tankWidth2-2, (int)-current2, Qt::blue);
    QPointF point;
    point.setX(originX);
    point.setY(originY+(tankHeight/2)-objectRadius);
    p.setBrush(Qt::red);
    p.drawEllipse(point,objectRadius,objectRadius);

    p.end();
}

void MainWindow::on_pushButton_stop_clicked()
{
    if (false == mThread->mThreadStop)
    {
        mThread->mThreadStop = true;
    }
}

void MainWindow::onSetLabel(QString label)
{
    ui->statusBar->showMessage(label);
}

void MainWindow::onUpdateSimulation(double input, double setpoint)
{
    update();
    Q_UNUSED(input);
    Q_UNUSED(setpoint);
    // add IRRange to lines:
    //ui->customPlot->graph(0)->addData(key, IRRange);

    // remove IRRange that's outside visible range:
    //ui->customPlot->graph(0)->removeDataBefore(key-15);
}

void MainWindow::on_spinBox_setpoint_valueChanged(double arg1)
{
    QPaintEvent *event;
    Q_UNUSED(event);
    mThread->setpoint = arg1;
    update();
}

void MainWindow::on_spinBox_setpoint2_valueChanged(double arg1)
{
    QPaintEvent *event;
    Q_UNUSED(event);
    mThread->setpoint2 = arg1;
    update();
}

void MainWindow::plotInit()
{
#if 0
    /* Blue - setpoint line
     * Red -  current height line */
    ui->customPlot->addGraph();
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, "IR Sensor"));
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(1)->setAntialiasedFill(false);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->yAxis->setRange(0, 20);
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Height (cm)");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
#endif
}

void MainWindow::on_pushButton_drop_clicked()
{
    mThread->objectDropped = true;
    ui->statusBar->showMessage("Object Dropped");
    mThread->objectRadius = qPow((0.75 * mThread->cmRadius * mThread->cmRadius * mThread->jump), 0.3333);
    update();
}
