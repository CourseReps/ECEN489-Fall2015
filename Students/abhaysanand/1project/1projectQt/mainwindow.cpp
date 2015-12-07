#include "mainwindow.h"
#include "ui_mainwindow.h"

QString dbaddr = "/home/mjolnir/data.db";
QString CurrentIP;
QSqlDatabase teensy_db = QSqlDatabase::addDatabase("QSQLITE");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mThread = new btThread(this);
    cThread = new clientThread(this);

    mThread->btThreadStop = true;
    cThread->clientThreadStop = true;

    MainWindow::easyplot();

    QString tableCreate = "CREATE TABLE IF NOT EXISTS Team1( ID TEXT, IRRange DOUBLE, PumpRate DOUBLE, FlowRate DOUBLE, SolenoidState TEXT, Timestamp INTEGER, CurrentIP TEXT);";

    if(databaseCreator(tableCreate) == false)
    {
        ui->label_success->setText("Database creation failed!");
    }

    //QTcpSocket socket;
    //socket.connectToHost("8.8.8.8", 53); // google DNS, or something else reliable

    //if (socket.waitForConnected())
    {
        //CurrentIP = socket.localAddress();
        CurrentIP = "123.123.123.132";

    }


    connect(mThread, SIGNAL(setLabel(QString)), this, SLOT(onSetLabel(QString)));
    connect(mThread, SIGNAL(setParameterValues(double,double,double,bool)), this, SLOT(onSetParameterValues(double,double,double,bool)));
    connect(cThread, SIGNAL(setLabelClient(QString)), this, SLOT(onsetLabelClient(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSetLabel(QString label)
{
    ui->label_threadStatus->setText(label);
}

void MainWindow::onsetLabelClient(QString label)
{
    ui->label_ServerState->setText(label);
}

void MainWindow::onSetParameterValues(double IRRange1, double PumpRate1, double FlowRate1, double PumpRate2, double IRRange2,bool SolenoidState_b)
{
    QString ID = "Team1";
    QDateTime dateTime;
    QString SolenoidState;

    if(true == SolenoidState_b)
    {
        SolenoidState = "ON";
        ui->horizontalSlider->setValue(1);
    }
    else
    {
        SolenoidState = "OFF";
        ui->horizontalSlider->setValue(0);
    }


    int Timestamp = dateTime.toTime_t();

    QString tableUpdate = " INSERT INTO Team1 VALUES ('" +ID+ "', '" + IRRange + "', '"+ PumpRate + "', '"+ FlowRate +"', '"+ SolenoidState +"', '"+ Timestamp +"' , '" + CurrentIP + "')" ;
    if(databaseCreator(tableUpdate) == false)
    {
        ui->label_success->setText("Database updation failed!");
    }

    realtimeDataSlot(IRRange, PumpRate, FlowRate);

    QJsonObject jsonWrite;

    /*
     * "ID":"TeamName",
     * "IRRange":"xxx",
     * "PumpRate":"xxx",
     * "FlowRate":"xxx",
     * "SolenoidState":"ON/OFF",
     * "Timestamp":"Unix epoch",
     * "CurrentIP":"xx.xxx.xxx.xx"
     */
    jsonWrite.insert("ID", QJsonValue(ID));
    jsonWrite.insert("IRRange", QJsonValue(float(IRRange)));
    jsonWrite.insert("PumpRate", QJsonValue(float(PumpRate)));
    jsonWrite.insert("FlowRate", QJsonValue(float(FlowRate)));
    jsonWrite.insert("SolenoidState", QJsonValue(SolenoidState));
    jsonWrite.insert("Timestamp", QJsonValue(Timestamp));
    jsonWrite.insert("CurrentIP", QJsonValue(CurrentIP));

    qDebug() << jsonWrite;

    QJsonDocument doc(jsonWrite);

    QFile file("sensor.json");

    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::on_pushButton_start_clicked()
{
    /* If the Server IP text box is empty, threads will not start. Stop thread flags are set to true */
    if(NULL == ui->lineEdit->text())
    {
        QMessageBox::critical(this, tr("Error"), tr("Enter Server IP address"));
        mThread->btThreadStop = true;
        cThread->clientThreadStop = true;
    }
    else
    {
        /* If Server IP not NULL and stop thread flag is true, only then thread will be started. */
        if(true == mThread->btThreadStop)
        {
            mThread->btThreadStop = false;
            mThread->start();
        }

        if(true == cThread->clientThreadStop)
        {
            cThread->clientThreadStop = false;
            cThread->serverIP = ui->lineEdit->text();
            cThread->start();
        }
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    mThread->btThreadStop = true;
    cThread->clientThreadStop = true;
}

void MainWindow::easyplot()
{
    //IRSensor
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, "IR Sensor"));
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->yAxis->setRange(0, 20);
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Distance (cm)");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));




    //FlowSensor
    ui->customPlot_2->addGraph(); // red line
    ui->customPlot_2->plotLayout()->insertRow(0);
    ui->customPlot_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot_2, "Pump Rate"));
    ui->customPlot_2->graph(0)->setPen(QPen(Qt::red));
    ui->customPlot_2->graph(0)->setAntialiasedFill(false);

    ui->customPlot_2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_2->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_2->xAxis->setAutoTickStep(false);
    ui->customPlot_2->xAxis->setTickStep(2);
    ui->customPlot_2->axisRect()->setupFullAxesBox();

    ui->customPlot_2->yAxis->setRange(0, 15);
    ui->customPlot_2->xAxis->setLabel("Time");
    ui->customPlot_2->yAxis->setLabel("Liters per minute");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_2->yAxis2, SLOT(setRange(QCPRange)));





    //Pump1
    ui->customPlot_3->addGraph(); // green line
    ui->customPlot_3->plotLayout()->insertRow(0);
    ui->customPlot_3->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot_3, "Flow Sensor Rate"));
    ui->customPlot_3->graph(0)->setPen(QPen(Qt::green));
    ui->customPlot_3->graph(0)->setAntialiasedFill(false);

    ui->customPlot_3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_3->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_3->xAxis->setAutoTickStep(false);
    ui->customPlot_3->xAxis->setTickStep(2);
    ui->customPlot_3->axisRect()->setupFullAxesBox();

    ui->customPlot_3->yAxis->setRange(0, 15);
    ui->customPlot_3->xAxis->setLabel("Time");
    ui->customPlot_3->yAxis->setLabel("Liters per minute");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_3->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot_3->yAxis2, SLOT(setRange(QCPRange)));

}

void MainWindow::realtimeDataSlot(double IRRange, double PumpRate, double FlowRate)
{

     //static double lastPointKey = 0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

    // add IRRange to lines:
    ui->customPlot->graph(0)->addData(key, IRRange);

    // remove IRRange that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot->yAxis->setRange(0, 2);
    ui->customPlot->replot();




    // add FlowRate to lines:
    ui->customPlot_2->graph(0)->addData(key, PumpRate);

    // remove FlowRate data that's outside visible range:
    ui->customPlot_2->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_2->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_2->yAxis->setRange(0, 5);
    ui->customPlot_2->replot();




    // add PumpRate to lines:
    ui->customPlot_3->graph(0)->addData(key, FlowRate);

    // remove PumpRate data that's outside visible range:
    ui->customPlot_3->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_3->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_3->yAxis->setRange(0, 5);
    ui->customPlot_3->replot();

}

bool MainWindow::databaseCreator(QString queryString)
{
    teensy_db.setDatabaseName(dbaddr);
    if (!teensy_db.open())
    {
        qDebug() << "Could not open Database file:";
        qDebug() << teensy_db.lastError();
        return 0;
     }

    else
    {
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
        teensy_db.close();
        delete query;
        return (ret1 & ret2);
     }

}
