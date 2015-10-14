#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_time=0;
    Data = -1;
    dbaddr = "/home/karticbhargav/workspace/data.db";
    MainWindow::easyplot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::easyplot()
{
    //IRSensor
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setAntialiasedFill(false);

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
    ui->customPlot_2->addGraph(); // red line
    ui->customPlot_2->graph(0)->setPen(QPen(Qt::red));
    ui->customPlot_2->graph(0)->setAntialiasedFill(false);

    ui->customPlot_2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_2->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_2->xAxis->setAutoTickStep(false);
    ui->customPlot_2->xAxis->setTickStep(2);
    ui->customPlot_2->axisRect()->setupFullAxesBox();

    ui->customPlot_2->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));





    //Pump
    ui->customPlot_3->addGraph(); // green line
    ui->customPlot_3->graph(0)->setPen(QPen(Qt::green));
    ui->customPlot_3->graph(0)->setAntialiasedFill(false);

    ui->customPlot_3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot_3->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot_3->xAxis->setAutoTickStep(false);
    ui->customPlot_3->xAxis->setTickStep(2);
    ui->customPlot_3->axisRect()->setupFullAxesBox();

    ui->customPlot_3->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void MainWindow::realtimeDataSlot(double IRRange, double FlowRate, double PumpRate)
{

     static double lastPointKey = 0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

    // add IRRange to lines:
    ui->customPlot->graph(0)->addData(key, IRRange);

    // remove IRRange that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot->replot();




    // add FlowRate to lines:
    ui->customPlot_2->graph(0)->addData(key, FlowRate);

    // remove FlowRate data that's outside visible range:
    ui->customPlot_2->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_2->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_2->replot();




    // add PumpRate to lines:
    ui->customPlot_3->graph(0)->addData(key, PumpRate);

    // remove PumpRate data that's outside visible range:
    ui->customPlot_3->graph(0)->removeDataBefore(key-15);

    // make key axis range scroll with the Data (at a constant range size of 8):
    ui->customPlot_3->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
    ui->customPlot_3->replot();



}

bool MainWindow::databaseCreator(QString queryString)
{

        teensy_db = QSqlDatabase::addDatabase("QSQLITE");
        teensy_db.setDatabaseName(dbaddr);
        if (!teensy_db.open())
        {
            qDebug() << "Could not open Database file:";
            qDebug() << teensy_db.lastError();
            return;
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
                     /* Load queryModel to display contents in tableView */
                     queryModel->setQuery(*query);

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

void MainWindow::bluetoothDataUpdate()
{

    QString ID = "Team1", SolenoidState, CurrentIP;
    double IRRange, FlowRate, PumpRate;
    int Timestamp = QDateTime::toTime_t();

    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS, or something else reliable
    if (socket.waitForConnected()) {

        QHostAddress myAddress = socket.localAddress();
        CurrentIP = myAddress.toString();
    }



    QString tableCreate = "CREATE TABLE Team1( ID TEXT, IRRange DOUBLE, PumpRate DOUBLE, FlowRate DOUBLE, SolenoidState TEXT, Timestamp INTEGER, CurrentIP TEXT);";

    if(databaseCreator(tableCreate) == false)
        ui->label_success->setText("Database creation failed!");


    //Code to Receive Data via Bluetooth
    //4 String Values IRRange, PumpRate, FlowRate, SolenoidState

//    {
//      "ID":"TeamName",
//      "IRRange":"xxx",
//      "PumpRate":"xxx",
//      "FlowRate":"xxx",
//      "SolenoidState":"ON/OFF",
//      "Timestamp":"Unix epoch",
//      "CurrentIP":"xx.xxx.xxx.xx"
//    }



    QString tableUpdate = " INSERT INTO Team1 VALUES(' "+ID+" ',' " + IRRange + " ',' "+ PumpRate + " ',' "+ FlowRate +" ',' "+ SolenoidState +" ',' "+ Timestamp +" ',' " + CurrentIP + " ') " ;
    if(databaseCreator(tableUpdate) == false)
        ui->label_success->setText("Database updation failed!");

    realtimeDataSlot(IRRange, FlowRate, PumpRate);






}

void MainWindow::close_qry()
{
    checkqry=0;
}

void MainWindow::open_qry()
{
    checkqry=1;
}




void MainWindow::on_verticalSlider_sliderMoved(int position)
{

}
