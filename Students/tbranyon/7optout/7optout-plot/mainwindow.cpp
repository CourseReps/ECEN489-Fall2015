#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGraph(ui->plot);
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
    graphplot->axisRect()->setupFullAxesBox();
    connect(graphplot->xAxis, SIGNAL(rangeChanged(QCPRange)), graphplot->xAxis2, SLOT(setRange(QCPRange)));
    connect(graphplot->yAxis, SIGNAL(rangeChanged(QCPRange)), graphplot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::readData()
{
    //open database
    sqlite3 *db;
    sqlite3_open("../db1.db", &db);

    //read db, store keys in a vector
    sqlite3_exec(db, "SELECT * FROM TBL1;", readKeys, &entryKeys, NULL);

    double timestamp;
    //loop through vector of keys
    for(int x = 0; x < entryKeys.length(); ++x)
    {
        //select line from table by key and extract values
        long xTime = 0;
        int ADCreading = 0;
        ///@TODO

        //do calculations with extracted values
        timestamp = xTime - starttime;
        double val = 3.3 * ADCreading / 1023;

        //pushback new values into corresponding vectors
        times[x].push_back(timestamp);
        ADCvals[x].push_back(val);

        //assign new data to graph
        ui->plot->graph(x)->setData(times.at(x), ADCvals.at(x));
    }

    //reset and redraw graph
    ui->plot->xAxis->setRange(timestamp-5, 5);
    ui->plot->replot();

    //close database
    sqlite3_close(db);
}

int MainWindow::readKeys(void *NA, int argc, char **argv, char **colname)
{
    std::cout << colname[0];
    static_cast<QVector<QString> *>(NA)->push_back(colname[0]);
    return 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
