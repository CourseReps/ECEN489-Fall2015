#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVector>
#include <QSqlQueryModel>
#include <QFileInfo>
#include <QtCore>
#include  <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_time=0;
    data = -1;
    count_graph =0;
    dbaddr = "/home/uday/qtProjects/finaltrialqtplotting/sensorDatabase.db";
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(3);
    ui->customPlot->axisRect()->setupFullAxesBox();


    ui->customPlot->xAxis->setLabel("Time(s)");
    ui->customPlot->yAxis->setLabel("Temperature");
    ui->customPlot->yAxis->setRange(0, 400);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(500); // Interval 0 means to refresh as fast as possible

}



void MainWindow::graphplot(QCustomPlot *customPlot,int i)
{

    QColor randcolor;
    randcolor.setRgb(rand()%255, rand()%255, rand()%255);
    ui->customPlot->addGraph();
    ui->customPlot->graph(i)->setPen(QPen(randcolor));
    ui->customPlot->graph(i)->setScatterStyle(QCPScatterStyle::ssDisc);
}

void MainWindow::queryData(QCustomPlot *customPlot, QString deviceNum,double timeStamps, double tempVals)
{

        int i = dev_id.indexOf(dev_name);

        // add data to lines:
        ui->customPlot->graph(i)->queryData(timeStamps, tempVals);

        // rescale value (vertical) axis to fit the current data:
        ui->customPlot->graph(i)->rescaleValueAxis();


        // position x-axis on the right at x+10 and with the range of (x+10)-50 to (x+10)
        ui->customPlot->xAxis->setRange(timeStamps+10, 50, Qt::AlignRight);
        ui->customPlot->yAxis->setRange(0,1000);
        ui->customPlot->replot();
}

void MainWindow::realtimeDataSlot()
{

    QVector<Qstring> deviceNums[10000];
        QVector<double> timeStamps[10000];
        QVector<double> tempVals[10000];
    QSqlDatabase accounts_db = QSqlDatabase::addDatabase("QSQLITE");
        accounts_db.setDatabaseName(dbaddr);
        if (!accounts_db.open())
        {
            qDebug() << "Could not open database file:";
            qDebug() << accounts_db.lastError();
            return;
         }
        else
        {
            qDebug("Connected to the database");
        }


    Qstring temp = Qstring("select Data, Timestamp, DeviceID from sensorData ORDER BY Timestamp;");
    qDebug() << "Query:"<< temp;
    QSqlQuery query(temp);

    if( !query.exec() )
             qDebug() << query.lastError();
    else
             qDebug( "Qeuried successfully" );


    //count the current number of rows in database ordered by timestamp
            int  cur_last_row;
            if(query.last())   cur_last_row = query.at();

            qDebug()<<"current last row: "<<cur_last_row;

            if(cur_last_row > prev_last_row) {

                         /*Before adding data, check to see if new devices are added
                          * and add a curve for the device
                          **/

                         // if db is not specified, then default database of this program is choosen
                         QSqlQuery dev_query("SELECT  DISTINCT DeviceID FROM Client_IP ORDER BY Timestamp;");
                         if( !dev_query.exec() )
                           qDebug() << dev_query.lastError();
                         else
                           qDebug( "Qeury executed successfully" );

                         // find the number of distinct devices
                          int cur_num_dev;
                          if(dev_query.last())   cur_num_dev = dev_query.at() ;// dev_num counts from "zero"

                         qDebug( "number of distinct devices: " );
                         qDebug() << cur_num_dev;

                         if(cur_num_dev>num_dev){

                             for(int i=num_dev; i<cur_num_dev+1; i++){
                                  dev_query.seek(i);
                                  dev_id[i]=dev_query.value(0).toString();
                                  qDebug() << dev_id[i];
                                 // add the add_graph function
                                  addCurve(ui->qtplot,i);
                             }
                             num_dev=cur_num_dev;
                          }




                         /*  for(int i=prev_last_row; i<cur_last_row+1;i++){

                               query.seek(i);
                               x[i]=query.value(0).toDouble();
                               y[i]=query.value(1).toDouble();
                               qDebug()<<x[i]<<y[i];
                               a[i]=query.value(2).toString();
                                qDebug()<<a[i];
                               // add the current data to the corresponding curve
                               addData(ui->qtplot,a[i],x[i],y[i]);
                               sleep(1);
                           }
                           */

                         QString tempdev= query.value(2).toString();
                          tempVals[prev_last_row]=query.value(0).toDouble();
                          timeStamps[prev_last_row]=query.value(1).toDouble();
                          qDebug()<<x[prev_last_row]<<y[prev_last_row];
                          deviceNums[prev_last_row]=tempdev;
                          //dviceNums.push_back(query.value(1).toString());
                           queryData(ui->qtplot,deviceNums[prev_last_row],timeStamps[prev_last_row],tempVals[prev_last_row]);

                           prev_last_row +=1;
                           qDebug()<<"previous last row: "<<prev_last_row;
               }

    //count_graph = deviceNums.length();


/*
     data = 0;
     static double lastPointKey = 0;
     double value0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;;

     if (key-lastPointKey >= 1.4) // at most add point every 14 ms
     {
         if(checkqry)
             if(qry->next())
             {
                 data = qry->value(2).toDouble();
                 qDebug() << "value:"<< qry->value(2).toDouble();
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
                 QSqlDatabase::removeDatabase("sData" );
             }
//     key= timestamp-init_time;  */





    // add data to lines:
    //ui->customPlot->graph(count_graph)->addData(timeStamps[count_graph], tempVals[count_graph]);

    // set data of dots:
   // ui->customPlot->graph(1)->clearData();
    //ui->customPlot->graph(count_graph)->addData(timeStamps[count_graph], tempVals[count_graph]);

    //ui->customPlot->graph(1)->addData(key, value0);

    // remove data of lines that's outside visible range:
   // ui->customPlot->graph(0)->removeDataBefore(key-15);
    }
    // rescale value (vertical) axis to fit the current data:
//    ui->customPlot->graph(0)->rescaleValueAxis();
/*
    lastPointKey = key;

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
/*
void MainWindow::close_qry()
{
    checkqry=0;
}

void MainWindow::open_qry()
{
    checkqry=1;
}
*/
MainWindow::~MainWindow()
{
    delete ui;
}
