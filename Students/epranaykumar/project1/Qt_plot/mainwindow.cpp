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


int num_dev = 0;//  query.size() not supported with sqlite
int prev_last_row=0;
QVector<QString> dev_id(10);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

          setgraphs( ui->qtplot,"Time","Flowrate",14);
          setgraphs( ui->qtplot_2,"Time","Voltage",12);
          setgraphs( ui->qtplot_3,"Time","IRvalue",2);

       // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
          connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
          dataTimer.start(500);
}

void MainWindow::setgraphs(QCustomPlot *customPlot,QString xlabel,QString ylabel,int rg){

    // configure bottom axis to show date and time instead of number:
        customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
       customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    // set a more compact font size for bottom and left axis tick labels:
       customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
       customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

       customPlot->xAxis->setLabel(xlabel);
       customPlot->yAxis->setLabel( ylabel);
    // scale x-axis in steps of 3:
      customPlot->xAxis->setAutoTickStep(false);
      customPlot->xAxis->setTickStep(1);
      customPlot->axisRect()->setupFullAxesBox();
      customPlot->legend->setVisible(true);
      customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);

       customPlot->yAxis->setRange(0,rg);

      // make left and bottom axes transfer their ranges to right and top axes:
         connect( customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),customPlot->xAxis2, SLOT(setRange(QCPRange)));
         connect( customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),customPlot->yAxis2, SLOT(setRange(QCPRange)));

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
        //customPlot->graph(i)->rescaleValueAxis();


        // position x-axis on the right at x+10 and with the range of (x+10)-50 to (x+10)
        customPlot->xAxis->setRange(x+3, 15, Qt::AlignRight);
        customPlot->replot();
}


void MainWindow::realtimeDataSlot(){

      // assign the appropriate driver to the database
       QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

       double flowrate,voltage,irvalue,timestamp,relaystate;
       QString dev_name;

       // open the specified database
        db.setDatabaseName(  "/home/team2/Desktop/data.db");

        if(!db.open()){
             qDebug("Failed to open the database");
        }
        else{
             qDebug("Connected to the database");




        /* order the querries ordered by timestamp and compare previous last row with current_last row
         * select the querries greater than previous timestamp and
         * add the data to the graph corresponding to the device
         **/

        QString stmt =QString("SELECT  Flowrate, Voltage, IR_sensor, RelayState, DeviceID,Timestamp FROM DATA ORDER BY Timestamp;");
        qDebug() <<"Query:"<< stmt;
        QSqlQuery query;
        query.prepare(stmt);

        if( !query.exec() )
            qDebug() << query.lastError();
        else
            qDebug( "Qeury: Executed successfully" );

        //count the current number of rows in database ordered by timestamp
        int  cur_last_row;
        if(query.last())   cur_last_row = query.at();

        qDebug()<<"current last row: "<<cur_last_row;

        if(cur_last_row >= prev_last_row) {

                     /*Before adding data, check to see if new devices are added
                      * and add a curve for the device
                      **/

                     // if db is not specified, then default database of this program is choosen
                     QSqlQuery dev_query("SELECT  DISTINCT DeviceID FROM DATA ORDER BY Timestamp;");
                     if( !dev_query.exec() )
                       qDebug() << dev_query.lastError();
                     else
                       qDebug( "Qeury executed successfully" );

                     // find the number of distinct devices
                      int cur_num_dev;
                      if(dev_query.last())   cur_num_dev = dev_query.at() ;// dev_num counts from "zero"

                     qDebug() << "number of distinct devices: " << cur_num_dev;


                     if(cur_num_dev>=num_dev){

                         for(int i=num_dev; i<cur_num_dev+1; i++){
                              dev_query.seek(i);
                              dev_id[i]=dev_query.value(0).toString();
                              qDebug() << dev_id[i];
                             // add the add_graph function
                              addCurve(ui->qtplot,i);
                              addCurve(ui->qtplot_2,i);
                              addCurve(ui->qtplot_3,i);
                         }
                         num_dev=cur_num_dev+1;
                      }

                       query.seek(prev_last_row);
                       flowrate=query.value(0).toDouble();//flowrate,voltage,irvalue,timestamp,relaystate
                       voltage=query.value(1).toDouble();
                       irvalue=query.value(2).toDouble();
                       relaystate=query.value(3).toDouble();
                       timestamp=query.value(5).toDouble();
                       qDebug()<<flowrate<<voltage<<irvalue<<relaystate<< timestamp;

                       dev_name=query.value(4).toString();
                        qDebug()<<dev_name;
                       // add the current data to the corresponding curve
                       addData(ui->qtplot,dev_name,timestamp,flowrate);
                       addData(ui->qtplot_2,dev_name,timestamp,voltage);
                       addData(ui->qtplot_3,dev_name, timestamp,irvalue);

                       if(relaystate==0)  {ui->on->setStyleSheet("background-color:  #D1CCCC");
                       ui->off->setStyleSheet("background-color: red");}
                       if(relaystate==1)  {ui->on->setStyleSheet("background-color: red");
                       ui->off->setStyleSheet("background-color:  #D1CCCC");}


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
