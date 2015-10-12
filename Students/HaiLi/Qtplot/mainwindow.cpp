#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_time=0;
    memset(count, 0, sizeof(count[0][0]) * 5 * 2);
    idcount=0;
    sum=0;
    i=1;
    maxFlow_rate=12;
    maxPump_rate=12;
    maxDistance=12;
    dbaddr = "/home/team3/teensydata.db";
    MainWindow::easyplot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::easyplot()
{

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Flow rate");
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));

    ui->customPlot->yAxis->setRange(0, maxFlow_rate);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    ui->customPlot2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot2->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot2->xAxis->setAutoTickStep(false);
    ui->customPlot2->xAxis->setTickStep(2);
    ui->customPlot2->axisRect()->setupFullAxesBox();
    ui->customPlot2->xAxis->setLabel("Time");
    ui->customPlot2->yAxis->setLabel("Pump rate");
    ui->customPlot2->legend->setVisible(true);
    ui->customPlot2->legend->setBrush(QBrush(QColor(255,255,255,150)));

    ui->customPlot2->yAxis->setRange(0, maxPump_rate);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot2->yAxis2, SLOT(setRange(QCPRange)));
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible

    ui->customPlot3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot3->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot3->xAxis->setAutoTickStep(false);
    ui->customPlot3->xAxis->setTickStep(2);
    ui->customPlot3->axisRect()->setupFullAxesBox();
    ui->customPlot3->xAxis->setLabel("Time");
    ui->customPlot3->yAxis->setLabel("Distance");
    ui->customPlot3->legend->setVisible(true);
    ui->customPlot3->legend->setBrush(QBrush(QColor(255,255,255,150)));

    ui->customPlot3->yAxis->setRange(0, maxDistance);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot3->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot3->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::realtimeDataSlot()
{

     static double lastPointKey = 0;
     double value[5];
     double tem[5];
     value[0]=0;
     value[1]=0;
     value[2]=0;
     value[3]=0;
     value[4]=0;
     double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

     if (key-lastPointKey >= 1) // at most add point every 10 ms
     {

         accounts_db = QSqlDatabase::addDatabase("QSQLITE","db");
         accounts_db.setDatabaseName(dbaddr);
         if (!accounts_db.open())
         {
             qDebug() << "Could not open database file:";
             qDebug() << accounts_db.lastError();
             return;
          }
         qy= new QSqlQuery(accounts_db);
         qy->prepare("select DeviceID from TBL1");
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
         while(sum<idcount){
         QColor color;
         color.setRgb(rand()%255, rand()%255, rand()%255);

         ui->customPlot->addGraph();
         ui->customPlot->graph(sum)->setPen(QPen(color));
         ui->customPlot->graph(sum)->setAntialiasedFill(false);
         ui->customPlot->graph(sum)->setName(IDlist[sum]);
         ui->customPlot2->addGraph();
         ui->customPlot2->graph(sum)->setPen(QPen(color));
         ui->customPlot2->graph(sum)->setAntialiasedFill(false);
         ui->customPlot2->graph(sum)->setName(IDlist[sum]);
         ui->customPlot3->addGraph();
         ui->customPlot3->graph(sum)->setPen(QPen(color));
         ui->customPlot3->graph(sum)->setAntialiasedFill(false);
         ui->customPlot3->graph(sum)->setName(IDlist[sum]);
         sum++;}
         int i=0;
         while(i<idcount){
         qry[i] = new QSqlQuery(accounts_db);
         //open_qry[0]();
         qry[i]->setForwardOnly(true);

         if (!(qry[i]->prepare("select FlowRate, PumpRate, IR_range from TBL1 where DeviceID="+IDlist[i]+";")))
         {
             qDebug() << "Query failed:";
             qDebug() << qry[i]->lastError();
             return;
         }
         const bool res0 = qry[i]->exec( );
         Q_ASSERT( res0 );

         while(count[i][1]<count[i][0]){
             count[i][1]++;
             qry[i]->next();}
         count[i][1]=0;
         tem[i]=qry[i]->value(0).toDouble();
         if(qry[i]->next())
             {

                 count[i][0]++;
                 value[i] = qry[i]->value(0).toDouble();
                 if (value[i]!=0){
                 tem[i]=qry[i]->value(0).toDouble();
                 ts[i]=qry[i]->value(1).toDouble();
                 devid[i]=qry[i]->value(2).toDouble();}
                 qDebug() << "value[i]"<< qry[i]->value(0).toDouble();
                 qDebug()<<ts[i];
                 qDebug()<<devid[i];
             }
             else
             {
                 qry[i]->~QSqlQuery();

             }
         i++;}


    QSqlDatabase::removeDatabase("db" );


    value[0] = tem[0];
    value[1]=tem[1];
    value[2]=tem[2];
    value[3]=tem[3];
    value[4]=tem[4];
    int j=0;
    while(j<idcount){
    // add data to lines:
    ui->customPlot->graph(j)->addData(key, value[j]);
    ui->customPlot2->graph(j)->addData(key, ts[j]);
    qDebug()<<ts[j];
    ui->customPlot3->graph(j)->addData(key, devid[j]);

    // remove data of lines that's outside visible range:
    ui->customPlot->graph(j)->removeDataBefore(key-15);
    ui->customPlot2->graph(j)->removeDataBefore(key-15);
    ui->customPlot3->graph(j)->removeDataBefore(key-15);
    j++;}

    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
  ui->customPlot->replot();
  ui->customPlot2->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
  ui->customPlot2->replot();
  ui->customPlot3->xAxis->setRange(key+0.25, 15, Qt::AlignRight);
  ui->customPlot3->replot();
}





