#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(300, 250, 1500, 700);
    timer=0;
    stablestate_1 = 0;
    stablestate_2 = 0;
    check_stable_time_1=0;
    check_stable_time_2=0;
    start_estimate_volumn = 0;
    estimated_volumn = 0;
    item_volumn = 0;
    system = new Watersystem(INIT_H1/100.0,INIT_H2/100.0,D1/1000.0,D2/1000.0,D3/1000.0);
    setpoint1 = SETPOINT1;
    setpoint2 = SETPOINT2;
    controller1 = new PIDController(4000,800,0,setpoint1/100.0);
    controller2 = new PIDController(4000,800,0,setpoint2/100.0);
    plotvector<<ui->customPlot<<ui->customPlot_Vin1<<ui->customPlot_Vin2<<ui->customPlot_Vout1<<ui->customPlot_Vout2<<ui->customPlot_V3;
    setupRealtimeDataDemo(plotvector);
    setWindowTitle("QCustomPlot: realtimedata");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRealtimeDataDemo(QVector<QCustomPlot*> plotvector)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  for(int i=0;i<6;i++){

        plotvector[i]->addGraph(); // blue line
        plotvector[i]->graph(0)->setPen(QPen(Qt::blue));
        plotvector[i]->graph(0)->setAntialiasedFill(false);

        plotvector[i]->addGraph(); // red line
        plotvector[i]->graph(1)->setPen(QPen(Qt::red));
        plotvector[i]->graph(1)->setAntialiasedFill(false);


        plotvector[i]->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        plotvector[i]->xAxis->setDateTimeFormat("hh:mm:ss");
        plotvector[i]->xAxis->setAutoTickStep(false);
        plotvector[i]->xAxis->setTickStep(2);
        plotvector[i]->axisRect()->setupFullAxesBox();

        plotvector[i]->xAxis->setLabel("Time");
        QString label;

        switch(i)
        {
        case 0:
            label = "h";
            break;
        case 1:
            label = "Qin1";
            break;
        case 2:
            label = "Qin2";
            break;
        case 3:
            label = "Qout1";
            break;
        case 4:
            label = "Qout2";
            break;
        case 5:
            label = "Q12";
            break;
        default:
            break;
        }
        plotvector[i]->yAxis->setLabel(label);

        // make left and bottom axes transfer their ranges to right and top axes:
        connect(plotvector[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), plotvector[i]->xAxis2, SLOT(setRange(QCPRange)));
        connect(plotvector[i]->yAxis, SIGNAL(rangeChanged(QCPRange)), plotvector[i]->yAxis2, SLOT(setRange(QCPRange)));
}
        //setup sliders
        ui->setpoint1->setValue(SETPOINT1);
        ui->setpoint1->setRange(0,MAX_H1);
        ui->setpoint2->setValue(SETPOINT1);
        ui->setpoint2->setRange(0,MAX_H2);
        ui->d1->setValue(D1);
        ui->d1->setRange(0,MAX_D1);
        ui->d2->setValue(D2);
        ui->d2->setRange(0,MAX_D2);
        ui->d3->setValue(D3);
        ui->d3->setRange(0,MAX_D3);


        //setup spinbox
        ui->spinBox_1->setMaximum(MAX_H1);
        ui->spinBox_1->setMinimum(0);
        ui->spinBox_1->setValue(SETPOINT1);
        ui->spinBox_2->setMaximum(MAX_H2);
        ui->spinBox_2->setMinimum(0);
        ui->spinBox_2->setValue(SETPOINT2);
        ui->d1_box->setMaximum(MAX_D1);
        ui->d1_box->setMinimum(0);
        ui->d1_box->setValue(D1);
        ui->d2_box->setMaximum(MAX_D2);
        ui->d2_box->setMinimum(0);
        ui->d2_box->setValue(D2);
        ui->d3_box->setMaximum(MAX_D3);
        ui->d3_box->setMinimum(0);
        ui->d3_box->setValue(D3);

        ui->Volumn_box->setMaximum(int(MAX_H1*Pi*R1*R1*10000));
        ui->Volumn_box->setMinimum(0);
        ui->Volumn_box->setValue(0);

          // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
          connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
          connect(ui->setpoint1, SIGNAL(valueChanged(int)), this, SLOT(value1(int)));
          connect(ui->setpoint2, SIGNAL(valueChanged(int)), this, SLOT(value2(int)));
          connect(ui->spinBox_1,SIGNAL(valueChanged(int)),ui->setpoint1,SLOT(setValue(int)));
          connect(ui->setpoint1,SIGNAL(valueChanged(int)),ui->spinBox_1,SLOT(setValue(int)));
          connect(ui->spinBox_2,SIGNAL(valueChanged(int)),ui->setpoint2,SLOT(setValue(int)));
          connect(ui->setpoint2,SIGNAL(valueChanged(int)),ui->spinBox_2,SLOT(setValue(int)));

          connect(ui->d1, SIGNAL(valueChanged(int)), this, SLOT(value_d1(int)));
          connect(ui->d1_box,SIGNAL(valueChanged(int)),ui->d1,SLOT(setValue(int)));
          connect(ui->d1,SIGNAL(valueChanged(int)),ui->d1_box,SLOT(setValue(int)));

          connect(ui->d2, SIGNAL(valueChanged(int)), this, SLOT(value_d2(int)));
          connect(ui->d2_box,SIGNAL(valueChanged(int)),ui->d2,SLOT(setValue(int)));
          connect(ui->d2,SIGNAL(valueChanged(int)),ui->d2_box,SLOT(setValue(int)));

          connect(ui->d3, SIGNAL(valueChanged(int)), this, SLOT(value_d3(int)));
          connect(ui->d3_box,SIGNAL(valueChanged(int)),ui->d3,SLOT(setValue(int)));
          connect(ui->d3,SIGNAL(valueChanged(int)),ui->d3_box,SLOT(setValue(int)));

          connect(ui->Volumn_box,SIGNAL(valueChanged(int)),this,SLOT(change_volumn(int)));


          dataTimer.start(0); // Interval 0 means to refresh as fast as possible


}

void MainWindow::realtimeDataSlot()
{
      // calculate two new data points:
    #if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
      double key = 0;
    #else
      double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    #endif
      static double lastPointKey = 0;
      if(timer==0)
      {
          system->pumpvolt1=controller1->control(system->h1);
          system->pumpvolt2=controller2->control(system->h2);
          system->flowrate1=system->volt_to_flowrate(system->pumpvolt1);
          system->flowrate2=system->volt_to_flowrate(system->pumpvolt2);
      }
      if (key-lastPointKey > 0.01) // at most add point every 10 ms
      {
        QVector<double> value;

        system->compute();

        value<<system->h1*100<<system->h2*100;
        addData(plotvector[0],value,key);
        value.clear();
        value<<system->Qin1*1000000;
        addData(plotvector[1],value,key);
        value.clear();
        value<<system->Qin2*1000000;
        addData(plotvector[2],value,key);
        value.clear();
        value<<system->Qout1*1000000;
        addData(plotvector[3],value,key);
        value.clear();
        value<<system->Qout2*1000000;
        addData(plotvector[4],value,key);
        value.clear();
        value<<system->Q3*1000000;
        addData(plotvector[5],value,key);
        value.clear();

        if(check_stablestate(1))
        {
            ui->textBrowser_1->setText("stable");
            stablestate_1 = true;
        }
        else
        {
            ui->textBrowser_1->setText("not stable");
            stablestate_1 = false;
        }
        if(check_stablestate(2))
        {
            ui->textBrowser_2->setText("stable");
            stablestate_2 = true;
        }
        else
        {
            ui->textBrowser_2->setText("not stable");
            stablestate_2 = false;
        }

        if(start_estimate_volumn)
        {
            compute_volumn();
        }
//        cout<<"estimated volumn: "<<estimated_volumn<<endl;
        ui->estimated_value->setText(QString::number(estimated_volumn));
        if(start_estimate_volumn==0&&estimated_volumn!=0)
            ui->relative_error->setText(QString::number((item_volumn-estimated_volumn)/item_volumn*100));

        lastPointKey = key;


        timer++;
        if(timer==100){timer=0;}

      }

    addCurve(plotvector[0],20,20,key);
    addCurve(plotvector[1],300,300,key);
    addCurve(plotvector[2],300,300,key);
    addCurve(plotvector[3],300,300,key);
    addCurve(plotvector[4],300,300,key);
    addCurve(plotvector[5],300,300,key);


}

void MainWindow::addData(QCustomPlot* customPlot,QVector<double> value,double key)
{
    for(int i=0;i<value.count();i++)
    {
        // add data to lines:
        customPlot->graph(i)->addData(key, value[i]);
        // remove data of lines that's outside visible range:
        customPlot->graph(i)->removeDataBefore(key-8);
        // rescale value (vertical) axis to fit the current data:
        customPlot->graph(i)->rescaleValueAxis();
    }
}

void MainWindow::addCurve(QCustomPlot* customPlot,double upper_level, double range,double key)
{
    // make key axis range scroll with the data (at a constant range size of 8):
    customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    customPlot->yAxis->setRange(upper_level, range, Qt::AlignRight);
    customPlot->replot();
}
void MainWindow::value1(int value)
{
    controller1->setSetpoint(value/100.0);
    setpoint1 = value;
//    cout<<value<<endl;
}

void MainWindow::value2(int value)
{
    controller2->setSetpoint(value/100.0);
    setpoint2 = value;
//    cout<<value<<endl;
}

void MainWindow::value_d1(int value)
{
    system->set_d1(value/1000.0);
}
void MainWindow::value_d2(int value)
{
    system->set_d2(value/1000.0);
}
void MainWindow::value_d3(int value)
{
    system->set_d3(value/1000.0);
}

bool MainWindow::check_stablestate(int flag)
{
    int* ptime;
    double h;
    double setpoint;
    if(flag==1)
    {
       ptime = &check_stable_time_1;
       h=system->h1*100;
       setpoint=setpoint1;
    }
    else if(flag==2)
    {
        ptime = &check_stable_time_2;
        h=system->h2*100;
        setpoint=setpoint2;
    }

    if(abs(h-setpoint)<stable_err)
    {
        (*ptime)++;
        if(*ptime<100)
            return false;
        else
            return true;
    }
    else
    {
        *ptime=0;
        return false;
    }
}

void MainWindow::change_volumn(int volumn)
{
    item_volumn = volumn;
}

void MainWindow::put_item(int volumn)  //ml
{
    if(system->put_item(volumn/1000000.0)==-1)
    {
        cout<<"too large!"<<endl;
    }
    else
        cout<<"Put item successfully!"<<endl;
}

void MainWindow::on_pushButton_clicked()
{
    if(stablestate_1)
    {
        put_item(item_volumn);
        start_estimate_volumn = 1;
        estimated_volumn = 0;
    }
    else
        cout<<"not stable now!"<<endl;
}

void MainWindow::compute_volumn()
{
    if(!stablestate_1)
    {
        estimated_volumn += (-system->dQ1) * 0.01*1000000;
    }
    else
    {
        cout<<"estimated over!"<<endl;
        start_estimate_volumn = 0;
    }
}
