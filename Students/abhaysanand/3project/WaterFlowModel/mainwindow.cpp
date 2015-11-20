#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mThread = new modelThread(this);

    connect(mThread, SIGNAL(setLabel(QString)), this, SLOT(onSetLabel(QString)));
    connect(mThread, SIGNAL(updateSimulation()), this, SLOT(onUpdateSimulation()));

    mThread->mThreadStop = true;
    mThread->input = 0;
    mThread->output = 0;
    mThread->setpoint = ui->spinBox_setpoint->value();
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

    int x = ui->widget->x();
    int y = ui->widget->y();
    int width = ui->widget->width();
    int height = ui->widget->height();
    int tankWidth = width/2;
    int tankHeight = height/2;
    int originX = x + width/2;
    int originY = y + height/2;

    double setpoint = mThread->setpoint;
    double current = mThread->input;

    double cmHeight = 16;
    double cmRadius = (cmHeight * tankWidth) / (tankHeight * 2);

    mThread->cmRadius = cmRadius;
    mThread->cmHeight = cmHeight;
    mThread->volume = PI * cmRadius * cmRadius * cmHeight;

    if (setpoint < 2)
    {
        setpoint = 2;
    }
    else if (setpoint > 15)
    {
        setpoint = 15;
    }

    setpoint = setpoint*tankHeight/cmHeight;
    current = current*tankHeight/cmHeight;
    //ui->statusBar->showMessage(QString::tos(setpoint));

    QPainter p;
    p.begin(this);
    p.drawRect(x,y,width,height);
    /* Left line */
    p.drawLine(originX-(tankWidth/2), originY-(tankHeight/2), originX-(tankWidth/2), originY+(tankHeight/2));
    /* Right line */
    p.drawLine(originX+(tankWidth/2), originY-(tankHeight/2), originX+(tankWidth/2), originY+(tankHeight/2));
    /* Bottom line */
    p.drawLine(originX-(tankWidth/2), originY+(tankHeight/2), originX+(tankWidth/2), originY+(tankHeight/2));
    /* Setpoint line */
    if((current >= (setpoint-1)) && (current <= (setpoint+1)))
    {
        p.fillRect(originX-(tankWidth/2), originY+(tankHeight/2)-(int)setpoint-1, -15, 3, Qt::green);
    }
    else if((current >= (setpoint-10)) && (current <= (setpoint+10)))
    {
        p.fillRect(originX-(tankWidth/2), originY+(tankHeight/2)-(int)setpoint-1, -15, 3, Qt::darkYellow);
    }
    else
    {
        p.fillRect(originX-(tankWidth/2), originY+(tankHeight/2)-(int)setpoint-1, -15, 3, Qt::red);
    }
    /* Water height */
    p.fillRect(1+originX-(tankWidth/2), originY+(tankHeight/2), tankWidth-2, (int)-current, Qt::blue);
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

void MainWindow::onUpdateSimulation()
{
    update();
}

void MainWindow::on_spinBox_setpoint_valueChanged(double arg1)
{
    QPaintEvent *event;
    Q_UNUSED(event);
    mThread->setpoint = arg1;
    update();
}
