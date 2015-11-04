#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

vector<pair<string, map<int, vector<double> > > >  globdata;
vector<int> numPlots;
string currentString="";
string btname = "~/hello.txt";


string MainWindow::getDBLoc(){return dbloc;}

void MainWindow::setDBLoc(string loc){dbloc = loc;}

void MainWindow::writeToBT(int x){
    string comm = "echo \"";
    comm+=to_string(x);
    comm+="\" | sudo tee "+btname+"\n";
    system(comm.c_str());
}

void MainWindow::addToGlobal(string id, pair<int, vector<double> > dta){
    bool isFound = false;
        for(int i=0; i<globdata.size(); i++){
            if(strcmp(globdata[i].first.c_str(),id.c_str())==0){
                isFound=true;
                globdata[i].second[dta.first]=dta.second;
                break;
            }
        }
        if(isFound==false){
            map<int, vector<double> > temp;
            temp[dta.first]=dta.second;
            pair<string, map<int, vector<double> > > temppair(id, temp);
            globdata.push_back(temppair);
        }
}

    int MainWindow::callback(void *NotUsed, int argc, char **argv, char **azColName){
        string ID = argv[0];
            vector<double> dt;
            for(int i=1; i<argc-1; i++){
                cout<<i<<":\t"<<argv[i]<<"\n";
                dt.push_back(stod(argv[i]));
            }
            pair<int, vector<double> > tp(stoi(argv[5]), dt);
            addToGlobal(ID, tp);

            return 0;
      }

   void MainWindow::refreshGlobalDB(){
       sqlite3 *db;
       char *zErrMsg = 0;
       int rc;

       rc = sqlite3_open(dbloc.c_str(), &db);
       if( rc ){
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         sqlite3_close(db);
         return;
       }
       rc = sqlite3_exec(db, "SELECT * FROM SensorData", callback, 0, &zErrMsg);
       if( rc!=SQLITE_OK ){
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
       }
       sqlite3_close(db);

     }

void MainWindow::addCurve(QCustomPlot *customPlot, int pn){
    numPlots[pn]++;
    QColor color;
    srand(time(0)+numPlots[pn]*rand());
    color.setRgb(rand()%255, rand()%255, rand()%255);
    customPlot->addGraph();
    customPlot->graph(numPlots[pn]-1)->setPen(QPen(color));
   // int i = globdata[numPlots-1].first;
    string n = "ID: "+globdata[numPlots[pn]-1].first;
    QString name = QString(n.c_str());
    customPlot->graph(numPlots[pn]-1)->setName(name);
    cout<<"Added curve # "<<numPlots[pn]<<" to plot "<<pn<<"\n";
}

void MainWindow::setUpPlot(QCustomPlot *customPlot){
    numPlots.push_back(0);
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

}

void MainWindow::realtimeDataSlot(){
    // add any necessary new curves
    refreshGlobalDB();

    if(globdata.size()>numPlots[0]){
        for(int p=0; p<numPlots[0]-globdata.size(); p++){
            addCurve(ui->widget, 0);
            addCurve(ui->widget_2, 1);
            addCurve(ui->widget_3, 2);
            addCurve(ui->widget_4, 3);
        }
    }
    double maxkey=0;
    for(int j=0; j<globdata.size(); j++){
        static double lastPointKey = 0;
        std::map<int,vector<double> >::iterator it=globdata[j].second.end();
        it--;
        double key = it->first;
        if(key>maxkey)
            maxkey=key;
        if (key-lastPointKey > 0.01) // at most add point every 10 ms
        {
          double value0 = double(it->second[0]);
          double value1 = double(it->second[1]);
          double value2 = double(it->second[2]);
          double value3 = double(it->second[3]);
          // add data to lines:
          ui->widget->graph(j)->addData(key, value0);
          // remove data of lines that's outside visible range:
          ui->widget->graph(j)->removeDataBefore(key-10);
          // rescale value (vertical) axis to fit the current data:
          ui->widget->graph(j)->rescaleValueAxis();
          ui->widget_2->graph(j)->addData(key, value1);
          // remove data of lines that's outside visible range:
          ui->widget_2->graph(j)->removeDataBefore(key-10);
          // rescale value (vertical) axis to fit the current data:
          ui->widget_2->graph(j)->rescaleValueAxis();
          ui->widget_3->graph(j)->addData(key, value2);
          // remove data of lines that's outside visible range:
          ui->widget_3->graph(j)->removeDataBefore(key-10);
          // rescale value (vertical) axis to fit the current data:
          ui->widget_3->graph(j)->rescaleValueAxis();
          ui->widget_4->graph(j)->addData(key, value3);
          // remove data of lines that's outside visible range:
          ui->widget_4->graph(j)->removeDataBefore(key-10);
          // rescale value (vertical) axis to fit the current data:
          ui->widget_4->graph(j)->rescaleValueAxis();
        }

    }
    // make key axis range scroll with the data (at a constant range size of 10):
    ui->widget->xAxis->setRange(maxkey+0.25, 10, Qt::AlignRight);
    ui->widget->yAxis->setRange(-0.5,75);
    ui->widget->replot();
    ui->widget_2->xAxis->setRange(maxkey+0.25, 10, Qt::AlignRight);
    ui->widget_2->yAxis->setRange(0,500);
    ui->widget_2->replot();
    ui->widget_3->xAxis->setRange(maxkey+0.25, 10, Qt::AlignRight);
    ui->widget_3->yAxis->setRange(0,800);
    ui->widget_3->replot();
    ui->widget_4->xAxis->setRange(maxkey+0.25, 10, Qt::AlignRight);
    ui->widget_4->yAxis->setRange(-0.25,1.25);
    ui->widget_4->replot();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpPlot(ui->widget);
    setUpPlot(ui->widget_2);
    setUpPlot(ui->widget_3);
    setUpPlot(ui->widget_4);
    ui->widget->plotLayout()->insertRow(0);
    ui->widget->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widget, "IR Range"));
    ui->widget_2->plotLayout()->insertRow(0);
    ui->widget_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widget_2, "Pump Rate"));
    ui->widget_3->plotLayout()->insertRow(0);
    ui->widget_3->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widget_3, "Flow Rate"));
    ui->widget_4->plotLayout()->insertRow(0);
    ui->widget_4->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widget_4, "Solenoid"));
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(1000); // Interval 0 means to refresh as fast as possible
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    currentString = arg1.toStdString();
}

void MainWindow::on_pushButton_clicked()
{
    try{
        int k = stoi(currentString);
        writeToBT(k);
    }

    catch(exception e){
        cout<<"An int value must be specified!, not \""<<currentString<<"\"\n";
    }
}
