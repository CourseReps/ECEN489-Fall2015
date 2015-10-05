#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data=-1;
    MainWindow::init_sql();
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


    ui->customPlot->yAxis->setRange(0, 100);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
  static QSqlQuery query;
  if(data==-1)
        get_query_sql(&query);
  get_timestamp_sql(&query);
//  qDebug() << "timestamp:" << timestamp;
  // calculate two new data points:
  double key = timestamp/1000.0;
          //QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
  static double lastPointKey = 0;
  double value0;
  if (key-lastPointKey >= 1) // at most add point every 10 ms
  {
    if(query.next())
    {
        get_data_sql(&query);
        value0 = data;
//        qDebug() << "value:" << data;
    }
    else
        value0 = 0;

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

void MainWindow::init_sql()
{
    accounts_db = QSqlDatabase::addDatabase("QSQLITE");
    accounts_db.setDatabaseName("/home/wszk1992/Desktop/jsondata.db");
    if (!accounts_db.open())
    {
        qDebug() << "Could not open database file:";
        qDebug() << accounts_db.lastError();
        return;
    }
}

void MainWindow::get_query_sql(QSqlQuery* query)
{
    // Note: don't construct queries before you have a database!
    if (!(query->exec("select Data,Timestamp from Thermistor;")))
    {
        qDebug() << "Query failed:";
        qDebug() << query->lastError();
        return;
    }
}

void MainWindow::get_data_sql(QSqlQuery *query)
{
    data = query->value(0).toDouble();

}

void MainWindow::get_timestamp_sql(QSqlQuery *query)
{
    bool ok;
    timestamp = query->value(1).toDouble();
     qDebug() << "timestamp:"<< query->value(1).toULongLong(&ok);
}





