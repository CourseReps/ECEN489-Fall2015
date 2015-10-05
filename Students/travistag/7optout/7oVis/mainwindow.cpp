#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sqlite3.h>
#include <string>
#include <iostream>
using namespace std;

vector<pair<int, map<int, double > > >  globdata;
int numPlots=0;

void MainWindow::addToGlobal(int id, pair<int, double> dta){
    bool isFound = false;
    for(int i=0; i<globdata.size(); i++){
        if(globdata[i].first==id){
            isFound=true;
            globdata[i].second[dta.first]=dta.second;
            break;
        }
    }
    if(isFound==false){
        map<int, double> temp;
        temp[dta.first]=dta.second;
        pair<int, map<int, double> > temppair(id, temp);
        globdata.push_back(temppair);
    }
}

   int MainWindow::callback(void *NotUsed, int argc, char **argv, char **azColName){
    int ID = atoi(argv[0]);
    int ts = atoi(argv[3]);
    int dt = atoi(argv[2]);
    pair<int, double> tp(ts, double(dt));
    addToGlobal(ID, tp);

    return 0;
  }

  void MainWindow::refreshGlobalDB(){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("../../../../build/photodata.db", &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return;
    }
    rc = sqlite3_exec(db, "SELECT * FROM PhotoData", callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);

  }

void MainWindow::addCurve(QCustomPlot *customPlot){
    numPlots++;
    QColor color;
    srand(time(0)+globdata[numPlots-1].first);
    color.setRgb(rand()%255, rand()%255, rand()%255);
    customPlot->addGraph();
    customPlot->graph(numPlots-1)->setPen(QPen(color));
    int i = globdata[numPlots-1].first;
    QString name = QString("ID: %1").arg(i);
    customPlot->graph(numPlots-1)->setName(name);
    cout<<"Added curve # "<<numPlots<<"\n";
}

void MainWindow::setUpPlot(QCustomPlot *customPlot){
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignLeft);
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(1000); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot(){
    // add any necessary new curves
    refreshGlobalDB();

    if(globdata.size()>numPlots){
        for(int p=0; p<numPlots-globdata.size(); p++){
            addCurve(ui->widget);
        }
    }
    double maxkey=0;
    for(int j=0; j<globdata.size(); j++){
        static double lastPointKey = 0;
        std::map<int,double>::iterator it=globdata[j].second.end();
        it--;
        double key = it->first;
        if(key>maxkey)
            maxkey=key;
        if (key-lastPointKey > 0.01) // at most add point every 10 ms
        {
          double value0 = double(it->second);
          // add data to lines:
          ui->widget->graph(j)->addData(key, value0);
          // remove data of lines that's outside visible range:
          ui->widget->graph(j)->removeDataBefore(key-10);
          // rescale value (vertical) axis to fit the current data:
          ui->widget->graph(j)->rescaleValueAxis();
        }
    }
    // make key axis range scroll with the data (at a constant range size of 10):
    ui->widget->xAxis->setRange(maxkey+0.25, 10, Qt::AlignRight);
    ui->widget->yAxis->setRange(0,500);
    ui->widget->replot();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpPlot(ui->widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
