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
    connect(mThread, SIGNAL(setParameterValues(double)), this, SLOT(onSetParameterValues(double)));

    ui->statusBar->showMessage("Waiting for BT connection...", 0);
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

void MainWindow::on_slider_setpoint1_valueChanged(int value)
{
    double setpoint = 2;
    setpoint = ((double)value / 2) + 2;

    mThread->IRSetpoint = setpoint;

    /* Set slider value to spinBox */
    ui->spin_setpoint->setValue(setpoint);
}

void MainWindow::on_slider_setpoint2_valueChanged(int value)
{

}

void MainWindow::onSetParameterValues(double IRCurrent)
{
    /* Set current value to slider and spinBox */
    int current = int((IRCurrent - 2) * 2);
    ui->slider_current->setValue(current);
    ui->spin_current->setValue(IRCurrent);
}



