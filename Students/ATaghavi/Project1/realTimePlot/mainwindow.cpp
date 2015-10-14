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
using namespace std;

map<string, vector< pair<string,string> > >sqlData;
map<string, QCustomPlot*> plots;

static int callback(void *data, int argc, char **argv, char **azColName){
   sqlData["IR Sensor"].push_back( pair<string,string>( argv[5], argv[1] ));
   sqlData["Flow Rate"].push_back( pair<string,string>( argv[5], argv[3] ));
   sqlData["Pump Rate"].push_back( pair<string,string>( argv[5], argv[2] ));
   sqlData["Solenoid"].push_back( pair<string,string>( argv[5], argv[4] ));
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

    QVBoxLayout *verticalLayout = ui->verticalLayout;
    readFromDB();
    QHBoxLayout *hlayout = new QHBoxLayout();
    verticalLayout->addLayout(hlayout);
    int k = 2;
    int p =0;
    for(map<string, vector< pair<string,string> > >::iterator it = sqlData.begin(); it != sqlData.end(); ++it) {
            vector<pair<string, string> > vals = it->second;

            if(p == k)
            {
                hlayout = new QHBoxLayout();
                verticalLayout->addLayout(hlayout);
                 p=0;
            }
             p++;
            QVector<double> x(vals.size()), y(vals.size());
            double maxx, minx;
            minx = maxx = atof(vals[0].first.c_str());
            for(int i=0; i<vals.size(); i++)
            {
                if(vals[i].second.find("ON") != string::npos)
                    y[i] = 100;
                else if(vals[i].second.find("OFF") != string::npos)
                    y[i] = 1;
                else
                    y[i] = atof(vals[i].second.c_str());

                x[i] = atof(vals[i].first.c_str());

                minx = min(minx, x[i]);
                maxx = max(maxx, x[i]);
            }
            QCustomPlot *customPlot2;
            if(plots[it->first] == NULL)
            {
             customPlot2 = new QCustomPlot();
             customPlot2->addGraph();
             plots[it->first] = customPlot2;
             customPlot2->plotLayout()->insertRow(0);
             customPlot2->plotLayout()->addElement(0,0, new QCPPlotTitle(customPlot2, QString(it->first.c_str())));
             customPlot2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
             customPlot2->xAxis->setDateTimeFormat("mm:ss");
             customPlot2->xAxis->setLabel("Time");
             customPlot2->yAxis->setLabel("Value");
             customPlot2->yAxis->setRange(0, 1000);
             hlayout->addWidget(customPlot2);
            }
            else
            {
             customPlot2 = plots[it->first];
            }
             customPlot2->graph(0)->setData(x, y);
             customPlot2->xAxis->setRange(maxx, 10, Qt::AlignRight);
             customPlot2->replot();     
       }
}

void MainWindow::sendVoltage(){
    int voltage = ui->spinBox->value();

    string s = "echo " + to_string(voltage) + " > /dev/cu.usbmodem1076261";

    system(s.c_str());
}

void MainWindow::setupPlayground(QCustomPlot *customPlot)
{
  Q_UNUSED(customPlot)
}

MainWindow::~MainWindow()
{
  delete ui;
}









































