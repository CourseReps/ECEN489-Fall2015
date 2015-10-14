#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data = -1;
    dbaddr = "/home/wszk1992/Desktop/jsondata.db";
    MainWindow::easyplot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::easyplot()
{

        ui->customPlot->addGraph(); // blue line
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setAntialiasedFill(false);

        ui->customPlot->addGraph(); // blue dot
        ui->customPlot->graph(1)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

        ui->customPlot->addGraph(); // blue line
        ui->customPlot->graph(2)->setPen(QPen(Qt::red));
        ui->customPlot->graph(2)->setAntialiasedFill(false);

        ui->customPlot->addGraph(); // blue dot
        ui->customPlot->graph(3)->setPen(QPen(Qt::red));
        ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

        ui->customPlot->addGraph(); // green line
        ui->customPlot->graph(4)->setPen(QPen(Qt::green));
        ui->customPlot->graph(4)->setAntialiasedFill(false);

        ui->customPlot->addGraph(); // green dot
        ui->customPlot->graph(5)->setPen(QPen(Qt::green));
        ui->customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);


    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();


    ui->customPlot->yAxis->setRange(0, 1000);

    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible

}

void MainWindow::realtimeDataSlot()
{

    if(data==-1)
    {
        accounts_db = QSqlDatabase::addDatabase("QSQLITE","zk");
        accounts_db.setDatabaseName(dbaddr);
        if (!accounts_db.open())
        {
            qDebug() << "Could not open database file:";
            qDebug() << accounts_db.lastError();
            return;
        }
 //       accounts_db.exec("pragma journal_mode=wal;");
    }

     static double lastPointKey = 0;

     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

     if (key-lastPointKey >= 0.01) // at most add point every 10 ms
     {
         qry = new QSqlQuery(accounts_db);
         qry->exec("select Data,DeviceID from Photosensor;");
         if(qry->last())
         {
             deviceID = qry->value(1).toString();
             qDebug() << "deviceID:"<< deviceID;

             if(data==-1)
             {
                 IDlist[0] = deviceID;
                 clientsum = 1;
             }

             //check if deviceID has being saved in IDlist
             for(clientnum=0;clientnum<clientsum;clientnum++)
             {
                 if(IDlist[clientnum]==deviceID)
                     break;
             }
             limit=0;
             if(clientnum==clientsum)
             {
                 if(clientnum<MAXCLIENT)
                 {
                     IDlist[clientnum]=deviceID;
                     clientsum+=1;
                 }
                 else
                     limit=1;
             }

             data = qry->value(0).toDouble();
             qDebug() << "value:"<< data;
         }
         else
         {
             qry->~QSqlQuery();
             accounts_db.close();
             QSqlDatabase::removeDatabase("zk" );
         }

         if(!limit)
         {
             value[clientnum] = data;
         }
         // add data to lines:
         ui->customPlot->graph(0)->addData(key, value[0]);
         // set data of dots:
         ui->customPlot->graph(1)->clearData();
         ui->customPlot->graph(1)->addData(key, value[0]);
         // remove data of lines that's outside visible range:
         ui->customPlot->graph(0)->removeDataBefore(key-15);

         // add data to lines:
         ui->customPlot->graph(2)->addData(key, value[1]);
         // set data of dots:
         ui->customPlot->graph(3)->clearData();
         ui->customPlot->graph(3)->addData(key, value[1]);
         // remove data of lines that's outside visible range:
         ui->customPlot->graph(2)->removeDataBefore(key-15);

        // add data to lines:
        ui->customPlot->graph(4)->addData(key, value[2]);
        // set data of dots:
        ui->customPlot->graph(5)->clearData();
        ui->customPlot->graph(5)->addData(key, value[2]);
        // remove data of lines that's outside visible range:
        ui->customPlot->graph(4)->removeDataBefore(key-15);


        lastPointKey = key;

     }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
  ui->customPlot->replot();

//// calculate frames per second:
//static double lastFpsKey;
//static int frameCount;
//++frameCount;
//if (key-lastFpsKey > 2) // average fps over 2 seconds
//{
//ui->statusBar->showMessage(
//      QString("%1 FPS, Total Data points: %2")
//      .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
//      .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
//      , 0);
//lastFpsKey = key;
//frameCount = 0;
//}
  data = 0;

}
