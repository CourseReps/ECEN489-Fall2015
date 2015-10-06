#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time(&starttime);
    setupGraph(ui->plot);
    sqlite3_open("../db1.db", &db);
    connect(&timer, SIGNAL(timeout()), this, SLOT(readData()));
    timer.start(1000); //update every second
}

void MainWindow::setupGraph(QCustomPlot *graphplot)
{
    graphplot->addGraph();
    graphplot->graph(0)->setPen(QPen(Qt::blue));

    graphplot->addGraph();
    graphplot->graph(1)->setPen(QPen(Qt::red));

    graphplot->addGraph();
    graphplot->graph(2)->setPen(QPen(Qt::green));

    graphplot->addGraph();
    graphplot->graph(3)->setPen(QPen(Qt::cyan));

    graphplot->addGraph();
    graphplot->graph(4)->setPen(QPen(Qt::magenta));

    graphplot->addGraph();
    graphplot->graph(5)->setPen(QPen(Qt::yellow));

    graphplot->addGraph();
    graphplot->graph(6)->setPen(QPen(Qt::darkGray));

    graphplot->addGraph();
    graphplot->graph(7)->setPen(QPen(Qt::black));

    graphplot->xAxis->setLabel("Time (s)");
    graphplot->yAxis->setLabel("ADC Volts");
    graphplot->xAxis->setRange(0,5);  //initialize to 5 second view
    graphplot->yAxis->setRange(0,3.3); //0-3.3 Volts (full scale)
    graphplot->xAxis->setAutoTickStep(false);
    graphplot->xAxis->setTickStep(1);
    //graphplot->axisRect()->setupFullAxesBox();
    //connect(graphplot->xAxis, SIGNAL(rangeChanged(QCPRange)), graphplot->xAxis2, SLOT(setRange(QCPRange)));
    //connect(graphplot->yAxis, SIGNAL(rangeChanged(QCPRange)), graphplot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::readData()
{
    //read db, store keys in a vector
    sqlite3_exec(db, "SELECT * FROM TBL1;", readKeys, &entryKeys, NULL);

    //loop through vector of keys
    double timestamp = 0;
    double ADCreading = -1;
    for(int x = 0; x < entryKeys.length(); ++x)
    {
        //select line from table by key and extract values
        char cmd[78] = {0};
        sprintf(cmd, "SELECT * FROM TBL1 WHERE deviceID = %s", entryKeys.at(x).toStdString().c_str());
        double datacont[2]; //timestamp, data
        sqlite3_exec(db, cmd, readVals, &datacont, 0);
        time_t xTime = datacont[0];
        ADCreading = datacont[1];

        //do calculations with extracted values
        timestamp = difftime(xTime, starttime);
        //ADCreading = std::rand() * 3.3/RAND_MAX; //debug
        ADCreading = ADCreading * 3.3/1023;
        //pushback new values into corresponding vectors
        std::cout << timestamp << " " << ADCreading << std::endl;
        if(timestamp < 0)
            return;
        times[x].push_back(timestamp);
        ADCvals[x].push_back(ADCreading);

        //assign new data to graph
        ui->plot->graph(x)->setData(times[x], ADCvals[x]);
        ui->plot->graph(x)->removeDataBefore(timestamp-5);
    }

    //reset and redraw graph
    ui->plot->xAxis->setRange(timestamp, 5, Qt::AlignRight);
    ui->plot->replot();
}

int MainWindow::readKeys(void *NA, int argc, char **argv, char **colname)
{
    QVector<QString> *ptr = static_cast<QVector<QString> *>(NA);
    if(!ptr->contains(argv[0]))
        ptr->push_back(argv[0]);

    return 0;
}

int MainWindow::readVals(void *NA, int argc, char **argv, char **colname)
{
    double xtimestamp = std::atof(argv[3]);
    static_cast<double*>(NA)[0] = xtimestamp;
    double xdata = std::atof(argv[2]);
    static_cast<double*>(NA)[1] = xdata;
    return 0;
}

MainWindow::~MainWindow()
{
    //close database
    sqlite3_close(db);

    delete ui;
}
