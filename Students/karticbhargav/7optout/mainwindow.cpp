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


    ui->customPlot->yAxis->setRange(0, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&DataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    DataTimer.start(0); // Interval 0 means to refresh as fast as possible

}

void MainWindow::realtimeDataSlot()
{

    if(Data==-1)
    {
        accounts_db = QSqlDatabase::addDatabase("QSQLITE","Database");
        accounts_db.setDatabaseName(dbaddr);
        if (!accounts_db.open())
        {
            qDebug() << "Could not open Database file:";
            qDebug() << accounts_db.lastError();
            return;
         }
        qry = new QSqlQuery(accounts_db);
        open_qry();
        qry->setForwardOnly(true);
        if (!(qry->prepare("select Data from Client_IP;")))
        {
            qDebug() << "Query failed:";
            qDebug() << qry->lastError();
            return;
        }
        const bool res = qry->exec( );
        Q_ASSERT( res );
        qry->first();
    }

     Data = 0;
     static double lastPointKey = 0;
     double value0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

     if (key-lastPointKey >= 1.4) // at most add point every 10 ms
     {
         if(checkqry)
             if(qry->next())
             {
                 Data = qry->value(0).toDouble();
                 qDebug() << "value:"<< qry->value(0).toDouble();
             }

         else
         {
             qry->~QSqlQuery();
             close_qry();
             accounts_db.close();
             QSqlDatabase::removeDatabase("Database" );
         }






    value0 = Data;

    // add Data to lines:
    ui->customPlot->graph(0)->addData(key, value0);

    // set Data of dots:
    ui->customPlot->graph(1)->clearData();
    ui->customPlot->graph(1)->addData(key, value0);

    // remove Data of lines that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);


    lastPointKey = key;
  }
  // make key axis range scroll with the Data (at a constant range size of 8):
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



