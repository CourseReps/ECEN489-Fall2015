/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <sqlite3.h>
#include <QTimer>
#include <vector>
#include <map>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

vector<map<string, vector< pair<string,string> > > >sqlData(5);
map<string, int> deviceIndices;
map<string, QCustomPlot*> plots;
int theIndex = 0;
vector<string> deviceNames;

string hostName = "127.0.0.1";
string portNumber =  "2222";

int getIndex(string name)
{
    bool found = false;
    for(int i=0; i<deviceNames.size(); i++)
        if(deviceNames[i] == name)
            found = true;

    if(deviceIndices[name] == NULL && !found)
    {
        cout<<"Adding new device named "<<name<<"\n";
        deviceIndices[name] = theIndex;
        plots["IR Sensor"]->addGraph();
        plots["Pump Rate"]->addGraph();
        plots["Flow Rate"]->addGraph();
        plots["Solenoid"]->addGraph();
        QColor color;
        srand(time(0)*rand());
        color.setRgb(rand()%255, rand()%255, rand()%255);
        plots["IR Sensor"]->graph(theIndex)->setPen(QPen(color));
        plots["IR Sensor"]->graph(theIndex)->setName(QString(name.c_str()));
        plots["Pump Rate"]->graph(theIndex)->setPen(QPen(color));
        plots["Pump Rate"]->graph(theIndex)->setName(QString(name.c_str()));
        plots["Flow Rate"]->graph(theIndex)->setPen(QPen(color));
        plots["Flow Rate"]->graph(theIndex)->setName(QString(name.c_str()));
        plots["Solenoid"]->graph(theIndex)->setPen(QPen(color));
        plots["Solenoid"]->graph(theIndex)->setName(QString(name.c_str()));
        theIndex++;
        deviceNames.push_back(name);
    }
    return deviceIndices[name];
}

void addPlot(string name, QHBoxLayout *hlayout, int minY,int maxY)
{
    QCustomPlot *customPlot = new QCustomPlot();
    plots[name] = customPlot;
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0,0, new QCPPlotTitle(customPlot, QString(name.c_str())));
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("mm:ss");
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Value");
    customPlot->yAxis->setRange(minY, maxY);
    /*
    for(int i=0; i<6; i++)
    {
        QColor color;
        srand(time(0)*rand());
        color.setRgb(rand()%255, rand()%255, rand()%255);
        customPlot->addGraph();
        customPlot->graph(i)->setPen(QPen(color));
    }
    */
    customPlot->legend->setVisible(true);
    hlayout->addWidget(customPlot);
}

void setUpPlots(QVBoxLayout *verticalLayout)
{
    QHBoxLayout *hlayout = new QHBoxLayout();
    verticalLayout->addLayout(hlayout);

    addPlot("IR Sensor", hlayout, 0,1200);
    addPlot("Pump Rate",hlayout,0,255);
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    verticalLayout->addLayout(hlayout2);
    addPlot("Flow Rate", hlayout2, 0,1000);
    addPlot("Solenoid",hlayout2,-1,2);
}

static int callback(void *data, int argc, char **argv, char **azColName){

   string s = argv[0];
   int t = getIndex(s);

   sqlData[t]["IR Sensor"].push_back( pair<string,string>( argv[5], argv[1] ));
   sqlData[t]["Flow Rate"].push_back( pair<string,string>( argv[5], argv[3] ));
   sqlData[t]["Pump Rate"].push_back( pair<string,string>( argv[5], argv[2] ));
   sqlData[t]["Solenoid"].push_back( pair<string,string>( argv[5], argv[4] ));
   return 0;
}


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(400*2, 250*2, 542*2, 390*2);
  setupDemo(0);
}

void MainWindow::setupDemo(int demoIndex)
{

  //setupSQLDemo(NULL, ui->verticalLayout);
  setWindowTitle("QCustomPlot: "+demoName);
  statusBar()->clearMessage();
  currentDemoIndex = demoIndex;
  setUpPlots(ui->verticalLayout);
  setupRealtimeDataDemo(NULL);
}


void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  demoName = "Real Time Data Demo";
   fprintf(stdout, "Operation asdfdone successfully\n");
  // include this section to fully disable antialiasing for higher performance:
  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  connect((ui->pushButton), SIGNAL(clicked()), this, SLOT(sendVoltage()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::readFromDB()
{

    sqlite3 *db;
       char *zErrMsg = 0;
       int rc;
       char *sql;
       const char* data = "Callback function called";
      rc = sqlite3_open("/Users/austin/dev/ECEN689/ECEN489-Fall2015/Students/ATaghavi/Project1/Project1.db", &db);
      if( rc ){
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         exit(0);
      }else{
      }

      /* Create SQL statement */
      sql = "SELECT * from SYSTEM_DATA";

      /* Execute SQL statement */
      rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
      if( rc != SQLITE_OK ){
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      }else{
         //fprintf(stdout, "Operation done successfully\n");
      }

      sqlite3_close(db);

}

void MainWindow::realtimeDataSlot()
{

  // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  double key = 0;
#else
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif

     readFromDB();
    for(int k=0; k<theIndex; k++)
    {

        map<string, vector< pair<string,string> > >myData = sqlData[k];

        bool first = true;
        double maxx, minx;
        for(map<string, vector< pair<string,string> > >::iterator it = myData.begin(); it != myData.end(); ++it) {
                vector<pair<string, string> > vals = it->second;
                QVector<double> x(vals.size()), y(vals.size());

                if(first)
                    minx = maxx = atof(vals[0].first.c_str());
                for(int i=0; i<vals.size(); i++)
                {
                    y[i] = atof(vals[i].second.c_str());

                    x[i] = atof(vals[i].first.c_str());

                    minx = min(minx, x[i]);
                    maxx = max(maxx, x[i]);
                }

                QCustomPlot *customPlot;
                 customPlot = plots[it->first];
                 customPlot->graph(k)->setData(x, y);
                 first = false;
           }
            plots["IR Sensor"]->xAxis->setRange(maxx, 50, Qt::AlignRight);
            plots["Pump Rate"]->xAxis->setRange(maxx, 50, Qt::AlignRight);
            plots["Flow Rate"]->xAxis->setRange(maxx, 50, Qt::AlignRight);
            plots["Solenoid"]->xAxis->setRange(maxx, 50, Qt::AlignRight);
        }
    plots["IR Sensor"]->replot();
    plots["Pump Rate"]->replot();
    plots["Flow Rate"]->replot();
    plots["Solenoid"]->replot();
}

void MainWindow::sendVoltage(){
    int voltage = ui->spinBox->value();

    //string s = "echo " + to_string(voltage) + " > /dev/cu.usbmodem1076261";

   // system(s.c_str());
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(hostName, portNumber);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);

    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(to_string(voltage)),
    boost::asio::transfer_all(), ignored_error);
}

void MainWindow::setupPlayground(QCustomPlot *customPlot)
{
  Q_UNUSED(customPlot)
}

MainWindow::~MainWindow()
{
  delete ui;
}









































