#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileInfo>
#include <QtCore>
#include  <unistd.h>

#include <time.h>

// query the database periodically
// update the curve for every deviceID found
// add new curve or plot if new deviceID is found

bool xLessThan( double p1, double p2){ return p1<p2;}
int num_dev = 0;//  query.size() not supported with sqlite
int prev_last_row=0;
QVector<QString> dev_id(10);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

       // configure bottom axis to show date and time instead of number:
           ui->qtplot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
           ui->qtplot->xAxis->setDateTimeFormat("hh:mm:ss");
       // set a more compact font size for bottom and left axis tick labels:
          ui->qtplot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
          ui->qtplot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

          ui->qtplot->xAxis->setLabel("Time");
          ui->qtplot->yAxis->setLabel("Data");
       // scale x-axis in steps of 3:
          ui->qtplot->xAxis->setAutoTickStep(false);
          ui->qtplot->xAxis->setTickStep(3);
          ui->qtplot->axisRect()->setupFullAxesBox();
          ui->qtplot->legend->setVisible(true);
          ui->qtplot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);

       // make left and bottom axes transfer their ranges to right and top axes:
          connect( ui->qtplot->xAxis, SIGNAL(rangeChanged(QCPRange)),ui->qtplot->xAxis2, SLOT(setRange(QCPRange)));
          connect( ui->qtplot->yAxis, SIGNAL(rangeChanged(QCPRange)),ui->qtplot->yAxis2, SLOT(setRange(QCPRange)));

       // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
          connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
          dataTimer.start(500);
}

void MainWindow::addCurve(QCustomPlot *customPlot,int i){

        QColor color;
        color.setRgb(rand()%255, rand()%255, rand()%255);
        customPlot->addGraph();
        customPlot->graph(i)->setPen(QPen(color));
        customPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
        QString name = QString("DeviceID: %1").arg(dev_id[i]);
        customPlot->graph(i)->setName(name);

}

void MainWindow::addData(QCustomPlot *customPlot, QString dev_name,double x, double y){

        int i = dev_id.indexOf(dev_name);

        // add data to lines:
        customPlot->graph(i)->addData(x, y);

        // rescale value (vertical) axis to fit the current data:
        customPlot->graph(i)->rescaleValueAxis();


        // position x-axis on the right at x+10 and with the range of (x+10)-50 to (x+10)
        customPlot->xAxis->setRange(x+10, 50, Qt::AlignRight);
        customPlot->yAxis->setRange(0,1000);
        customPlot->replot();
}


void MainWindow::realtimeDataSlot(){

      // assign the appropriate driver to the database
       QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

       QVector<double> x(2000),y(2000);
       QVector<QString> a(2000);

       // open the specified database
        db.setDatabaseName( "/home/pranaykumar/clionProjects/6optOut/serialServer/build/data.db");

        if(!db.open()){
             qDebug("Failed to open the database");
        }
        else{
             qDebug("Connected to the database");




        /* order the querries ordered by timestamp and compare previous last row with current_last row
         * select the querries greater than previous timestamp and
         * add the data to the graph corresponding to the device
         **/

        QString stmt =QString("SELECT Timestamp, Data, DeviceID FROM Client_IP ORDER BY Timestamp;");
        qDebug() <<"Query:"<< stmt;
        QSqlQuery query(stmt);

        if( !query.exec() )
            qDebug() << query.lastError();
        else
            qDebug( "Qeury: Executed successfully" );

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

                       query.seek(prev_last_row);
                       x[prev_last_row]=query.value(0).toDouble();
                       y[prev_last_row]=query.value(1).toDouble();
                       qDebug()<<x[prev_last_row]<<y[prev_last_row];
                       a[prev_last_row]=query.value(2).toString();
                        qDebug()<<a[prev_last_row];
                       // add the current data to the corresponding curve
                       addData(ui->qtplot,a[prev_last_row],x[prev_last_row],y[prev_last_row]);

                       prev_last_row +=1;
                       qDebug()<<"previous last row: "<<prev_last_row;
           }

            db.close();

     }//if connected to the database

}


MainWindow::~MainWindow()
{
    delete ui;
}
