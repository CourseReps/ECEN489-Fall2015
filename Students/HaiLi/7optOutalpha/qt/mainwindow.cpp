#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <qcustomplot.h>
#include <dbmanager.h>
QVector<double> a(200),b(200);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    static const QString path = "../Desktop/data.db";
    int i=0;
    DbManager db(path);
        QString x[2000],y[2000];
        //QVector<double> a(2000),b(2000);

            QSqlQuery query("SELECT * FROM Client_IP");
            int ID = query.record().indexOf("DeviceID");
            int d=query.record().indexOf("Data");
            int t=query.record().indexOf("Timestamp");
            while (query.next())
            {
                QString m=query.value(ID).toString();

                x[i] = query.value(d).toString();
                y[i]=query.value(t).toString();
                qDebug()<<x[i]<<y[i];
                a[i]=x[i].toDouble();
                b[i]=y[i].toDouble()-1443640000-293969+20000;
                qDebug()<<a[i]<<b[i];
                i++;

            }
//setupRealtimeDataDemo(ui->customPlot);
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(b, a);
        qDebug()<<a<<b;
        // give the axes some labels:
        ui->customPlot->xAxis->setLabel("x");
        ui->customPlot->yAxis->setLabel("y");
        // set axes ranges, so we see all data:
        //ui->customPlot->xAxis->setRange(1.44365e+09, 1.44366e+09);
        ui->customPlot->xAxis->setRange(0,10);
        ui->customPlot->yAxis->setRange(0,3.3);
        ui->customPlot->replot();
}

/*void MainWindow::easyplot()
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
void MainWindow::get_query_sql(QSqlQuery* query)
{
    // Note: don't construct queries before you have a database!
    if (!(query->exec("select Data,Timestamp from Client_IP;")))
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
}*/
MainWindow::~MainWindow()
{
    delete ui;
}
