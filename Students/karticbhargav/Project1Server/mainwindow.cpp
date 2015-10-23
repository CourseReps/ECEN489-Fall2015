#include "mainwindow.h"
#include "ui_mainwindow.h"

QString dbaddr;
QString CurrentIP;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mThread = new btThread(this);

    dbaddr = "/home/karticbhargav/dataServer.db";
    MainWindow::easyplot();

    QString tableCreate = "CREATE TABLE IF NOT EXISTS ServerTable( ID TEXT, IRRange DOUBLE, PumpRate DOUBLE, FlowRate DOUBLE, SolenoidState TEXT, Timestamp INTEGER, CurrentIP TEXT);";

    if(databaseCreator(tableCreate) == false)
    {
        ui->label_success->setText("Database creation failed!");
    }




    connect(mThread, SIGNAL(setParameterValues(double,double,double,bool,QString,int,QString,int)), this, SLOT(onSetParameterValues(double,double,double,bool, QString, int, QString, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onSetParameterValues(double IRRange, double PumpRate, double FlowRate, bool SolenoidState, QString ID, int Timestamp, QString CurrentIP, int i)
{

    QString tableUpdate = " INSERT INTO ServerTable VALUES(' "+ID+" ',' " + IRRange + " ',' "+ PumpRate + " ',' "+ FlowRate +" ',' "+ SolenoidState +" ',' "+ Timestamp +" ',' " + CurrentIP + " ') " ;
    if(databaseCreator(tableUpdate) == false)
    {
        ui->label_success->setText("Database updation failed!");
    }

    realtimeDataSlot(IRRange, FlowRate, PumpRate, i);

}

void MainWindow::on_pushButton_start_clicked()
{
    mThread->btThreadStop = false;
    mThread->start();
}

void MainWindow::on_pushButton_stop_clicked()
{
    mThread->btThreadStop = true;
}

void MainWindow::easyplot()
{
    //IRSensor
    for(int i=0; i<5; i++)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->setAntialiasedFill(false);

        switch(i)
        {
        case 0:
            ui->customPlot->graph(i)->setPen(QPen(Qt::red));
            break;

        case 1:
            ui->customPlot->graph(i)->setPen(QPen(Qt::blue));
            break;


        case 2:
            ui->customPlot->graph(i)->setPen(QPen(Qt::green));
            break;


        case 3:
            ui->customPlot->graph(i)->setPen(QPen(Qt::yellow));
            break;

        case 4:
            ui->customPlot->graph(i)->setPen(QPen(Qt::black));
            break;
        }
    }


    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));






    //FlowSensor
    for(int i=0; i<5; i++)
    {
        ui->customPlot_2->addGraph();
        ui->customPlot_2->graph(i)->setAntialiasedFill(false);

        switch(i)
        {
        case 0:
            ui->customPlot_2->graph(i)->setPen(QPen(Qt::red));
            break;

        case 1:
            ui->customPlot_2->graph(i)->setPen(QPen(Qt::blue));
            break;


        case 2:
            ui->customPlot_2->graph(i)->setPen(QPen(Qt::green));
            break;


        case 3:
            ui->customPlot_2->graph(i)->setPen(QPen(Qt::yellow));
            break;

        case 4:
            ui->customPlot_2->graph(i)->setPen(QPen(Qt::black));
            break;
        }
    }
    ui->customPlot_2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_2->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_2->xAxis->setAutoTickStep(false);
    ui->customPlot_2->xAxis->setTickStep(2);
    ui->customPlot_2->axisRect()->setupFullAxesBox();
    ui->customPlot_2->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_2->yAxis2, SLOT(setRange(QCPRange)));






    //PumpRate
    for(int i=0; i<5; i++)
    {
        ui->customPlot_3->addGraph();
        ui->customPlot_3->graph(i)->setAntialiasedFill(false);

        switch(i)
        {
        case 0:
            ui->customPlot_3->graph(i)->setPen(QPen(Qt::red));
            break;

        case 1:
            ui->customPlot_3->graph(i)->setPen(QPen(Qt::blue));
            break;


        case 2:
            ui->customPlot_3->graph(i)->setPen(QPen(Qt::green));
            break;


        case 3:
            ui->customPlot_3->graph(i)->setPen(QPen(Qt::yellow));
            break;

        case 4:
            ui->customPlot_3->graph(i)->setPen(QPen(Qt::black));
            break;
        }
    }
    ui->customPlot_3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_3->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_3->xAxis->setAutoTickStep(false);
    ui->customPlot_3->xAxis->setTickStep(2);
    ui->customPlot_3->axisRect()->setupFullAxesBox();
    ui->customPlot_3->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_3->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_3->yAxis2, SLOT(setRange(QCPRange)));

}

void MainWindow::realtimeDataSlot(double IRRange, double PumpRate, double FlowRate, int i)
{

     static double lastPointKey = 0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

    // add IRRange to lines:
    ui->customPlot->graph(i)->addData(key, IRRange);

    // remove IRRange that's outside visible range:
    ui->customPlot->graph(i)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot->yAxis->setRange(0, 20);
    ui->customPlot->replot();




    // add FlowRate to lines:
    ui->customPlot_2->graph(i)->addData(key, FlowRate);

    // remove FlowRate data that's outside visible range:
    ui->customPlot_2->graph(i)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_2->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_2->yAxis->setRange(0, 15);
    ui->customPlot_2->replot();




    // add PumpRate to lines:
    ui->customPlot_3->graph(i)->addData(key, PumpRate);

    // remove PumpRate data that's outside visible range:
    ui->customPlot_3->graph(i)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_3->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_3->yAxis->setRange(0, 15);
    ui->customPlot_3->replot();

}

bool MainWindow::databaseCreator(QString queryString)
{
    QSqlDatabase teensy_db = QSqlDatabase::addDatabase("QSQLITE");
    teensy_db.setDatabaseName(dbaddr);
    if (!teensy_db.open())
    {
        qDebug() << "Could not open Database file:";
        qDebug() << teensy_db.lastError();
        return 0;
     }

    else{
         QSqlQuery *query = new QSqlQuery(teensy_db);
         bool ret1 = 0, ret2 = 0;

         ui->label_success->setText("Opened");

         /* Prepare query to display contents of database */
         ret1 = query->prepare(queryString);
         if (ret1 == true)
         {
             /* Execute the query */
             ret2 = query->exec();
             if (ret2 == true)
             {
                 /* close database */
                 teensy_db.close();
             }
             else
             {
                 ui->label_success->setText("Execute command failed...");
             }
         }
         else
         {
             ui->label_success->setText("Prepare command failed...");
         }

         return (ret1 & ret2);
     }

}
