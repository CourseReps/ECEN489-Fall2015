#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_time=0;
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

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();


    ui->customPlot->yAxis->setRange(0, 400);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

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
        qry = new QSqlQuery(accounts_db);
        open_qry();
        qry->setForwardOnly(true);
        if (!(qry->prepare("select Data from Photosensor;")))
        {
            qDebug() << "Query failed:";
            qDebug() << qry->lastError();
            return;
        }
        const bool res = qry->exec( );
        Q_ASSERT( res );
        qry->first();
    }

     data = 0;
     static double lastPointKey = 0;
     double value0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

     if (key-lastPointKey >= 1.4) // at most add point every 10 ms
     {
         if(checkqry)
             if(qry->next())
             {
                 data = qry->value(0).toDouble();
                 qDebug() << "value:"<< qry->value(0).toDouble();
 //                timestamp = qry->value(1).toInt();
 //                qDebug() << "timestamp:"<< qry->value(1).toInt();
//                 if(init_time==0)
//                 {
//                     init_time = timestamp;
//                 }
             }
             else
             {
                 qry->~QSqlQuery();
                 close_qry();
                 accounts_db.close();
                 QSqlDatabase::removeDatabase("zk" );
             }
//     key= timestamp-init_time;





    value0 = data;

    // add data to lines:
    ui->customPlot->graph(0)->addData(key, value0);

    // set data of dots:
    ui->customPlot->graph(1)->clearData();
    ui->customPlot->graph(1)->addData(key, value0);

    // remove data of lines that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);

    // rescale value (vertical) axis to fit the current data:
//    ui->customPlot->graph(0)->rescaleValueAxis();

    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
  ui->customPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }

}

void MainWindow::close_qry()
{
    checkqry=0;
}

void MainWindow::open_qry()
{
    checkqry=1;
}



