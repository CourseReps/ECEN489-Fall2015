#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_time=0;
    data = -1;
    count11=0;
    count12=0;
    count21=0;
    count22=0;
    count31=0;
    count32=0;
    idcount=0;
    dbaddr = "../Desktop/task/6optOut/serialServer/build/data.db";
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
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(2)->setPen(QPen(Qt::red));
    ui->customPlot->graph(2)->setAntialiasedFill(false);

    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph(); // yellow line
    ui->customPlot->graph(4)->setPen(QPen(Qt::yellow));
    ui->customPlot->graph(4)->setAntialiasedFill(false);

    ui->customPlot->addGraph(); // yellow dot
    ui->customPlot->graph(5)->setPen(QPen(Qt::yellow));
    ui->customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);

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

    /*if(data==-1)
    {
        accounts_db = QSqlDatabase::addDatabase("QSQLITE","db");
        accounts_db.setDatabaseName(dbaddr);
        if (!accounts_db.open())
        {
            qDebug() << "Could not open database file:";
            qDebug() << accounts_db.lastError();
            return;
         }
        qry1 = new QSqlQuery(accounts_db);
        //open_qry1();
        qry1->setForwardOnly(true);
        if (!(qry1->prepare("select Data from Client_IP;")))
        {
            qDebug() << "Query failed:";
            qDebug() << qry1->lastError();
            return;
        }
        const bool res = qry1->exec( );
        Q_ASSERT( res );
        qry1->first();
        while(count2<count1){
            count2++;
            qry1->next();
        }
    }*/
     //count2=0;
     data = 0;
     static double lastPointKey = 0;
     double value0,tem0,tem1,tem2,value1,value2;
     value0=0;
     value1=0;
     value2=0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

     if (key-lastPointKey >= 1) // at most add point every 10 ms
     {
         /*qry1 = new QSqlQuery(accounts_db);
         qry1->setForwardOnly(true);
         if (!(qry1->prepare("select Data from Client_IP;")))
         {
             qDebug() << "Query failed:";
             qDebug() << qry1->lastError();
             return;
         }
         const bool res = qry1->exec( );
         Q_ASSERT( res );*/
         //qry1->first();
         accounts_db = QSqlDatabase::addDatabase("QSQLITE","db");
         accounts_db.setDatabaseName(dbaddr);
         if (!accounts_db.open())
         {
             qDebug() << "Could not open database file:";
             qDebug() << accounts_db.lastError();
             return;
          }
         qy= new QSqlQuery(accounts_db);
         qy->prepare("select DeviceID from Client_IP");
         qy->exec();
         QString temper;
         int q=0;
         int pj=0;
         while(qy->next()){
             temper=qy->value(0).toString();
             while(q<idcount){
                 if(IDlist[q]==temper){
                     pj=1;
                 }
                 q++;
             }
             q=0;
             if (pj==0){
                 IDlist[idcount]=temper;
                 idcount++;
             }
             else{
                 pj=0;
             }
         }
         qy->~QSqlQuery();
         qry1 = new QSqlQuery(accounts_db);
         //open_qry1();
         qry1->setForwardOnly(true);

         if (!(qry1->prepare("select Data from Client_IP where DeviceID="+IDlist[0]+";")))
         {
             qDebug() << "Query failed:";
             qDebug() << qry1->lastError();
             return;
         }
         const bool res0 = qry1->exec( );
         Q_ASSERT( res0 );
         //qry1->first();
         while(count12<count11){
             count12++;
             qry1->next();}
         count12=0;
         tem0=qry1->value(0).toDouble();
         if(qry1->next())
             {
                 //qDebug()<<qry1->last();
                 count11++;
                 value0 = qry1->value(0).toDouble();
                 if (value0!=0){
                 tem0=qry1->value(0).toDouble();}
                 qDebug() << "value0:"<< qry1->value(0).toDouble();
                 //QSqlDatabase::removeDatabase("db");
             }
             else
             {
                 qry1->~QSqlQuery();
                 //close_qry1();
                 //accounts_db.close();
                 //QSqlDatabase::removeDatabase("db" );
                 data=-1;
             }
//     key= timestamp-init_time;

         qry2 = new QSqlQuery(accounts_db);
         //open_qry2();
         qry2->setForwardOnly(true);
       if (idcount>1){
         if (!(qry2->prepare("select Data from Client_IP where DeviceID="+IDlist[1]+";")))
         {
             qDebug() << "Query failed:";
             qDebug() << qry2->lastError();
             return;
         }
         const bool res1 = qry2->exec( );
         Q_ASSERT( res1 );
         //qry2->first();
         while(count22<count21){
             count22++;
             qry2->next();}
         count22=0;
         tem1=qry2->value(0).toDouble();
         if(qry2->next())
             {
                 //qDebug()<<qry2->last();
                 count21++;
                 value1 = qry2->value(0).toDouble();
                 if (value1!=0){
                 tem1=qry2->value(0).toDouble();}
                 qDebug() << "value1:"<< qry2->value(0).toDouble();
                 //QSqlDatabase::removeDatabase("db");
             }
             else
             {
                 qry2->~QSqlQuery();
                 //close_qry2();
                 //accounts_db.close();
                 //QSqlDatabase::removeDatabase("db" );
                 data=-1;
             }}
       qry3 = new QSqlQuery(accounts_db);
       //open_qry1();
       qry3->setForwardOnly(true);
    if(idcount>2){
       if (!(qry3->prepare("select Data from Client_IP where DeviceID="+IDlist[2]+";")))
       {
           qDebug() << "Query failed:";
           qDebug() << qry3->lastError();
           return;
       }
       const bool res2 = qry3->exec( );
       Q_ASSERT( res2 );
       //qry1->first();
       while(count32<count31){
           count32++;
           qry3->next();}
       count32=0;
       tem2=qry3->value(0).toDouble();
       if(qry3->next())
           {
               //qDebug()<<qry1->last();
               count31++;
               value2 = qry3->value(0).toDouble();
               if (value2!=0){
               tem2=qry3->value(0).toDouble();}
               qDebug() << "value2:"<< qry3->value(0).toDouble();
               //QSqlDatabase::removeDatabase("db");
           }
           else
           {
               qry3->~QSqlQuery();
               //close_qry1();
               //accounts_db.close();
               //QSqlDatabase::removeDatabase("db" );
               data=-1;
           }}
    QSqlDatabase::removeDatabase("db" );


    value0 = tem0;
    value1=tem1;
    value2=tem2;
    // add data to lines:
    ui->customPlot->graph(0)->addData(key, value0);

    // set data of dots:
    ui->customPlot->graph(1)->clearData();
    ui->customPlot->graph(1)->addData(key, value0);

    // remove data of lines that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);
   if(idcount>1){
    ui->customPlot->graph(2)->addData(key, value1);
   // qDebug()<<value1;
    // set data of dots:
    ui->customPlot->graph(3)->clearData();
    ui->customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->customPlot->graph(2)->removeDataBefore(key-15);}
   if(idcount>2){
    ui->customPlot->graph(4)->addData(key, value2);
   // qDebug()<<value2;
    // set data of dots:
    ui->customPlot->graph(5)->clearData();
    ui->customPlot->graph(5)->addData(key, value2);

    // remove data of lines that's outside visible range:
    ui->customPlot->graph(4)->removeDataBefore(key-15);}

    // rescale value (vertical) axis to fit the current data:
//    ui->customPlot->graph(0)->rescaleValueAxis();

    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
  ui->customPlot->replot();

  /*// calculate frames per second:
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
  }*/

}





