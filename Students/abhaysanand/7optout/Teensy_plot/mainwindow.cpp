#include "mainwindow.h"
#include "ui_mainwindow.h"

QSqlDatabase teensy_db = QSqlDatabase::addDatabase("QSQLITE");

static QColor graphColor[10] = {Qt::black,Qt::red,Qt::blue,Qt::green,Qt::magenta,Qt::gray,
                         Qt::darkYellow,Qt::darkCyan,Qt::darkGray,Qt::darkGreen};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_browse_clicked()
{
    /* Browse for database file */
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database File"),"/path/to/file/",tr("Database Files (*.db)"));

    /* Display browsed file full path in lineEdit field */
    ui->lineEdit_fileLocation->setText(fileName);
}

bool MainWindow::queryDatabase(QSqlQueryModel *queryModel, QString queryString)
{
    teensy_db.setDatabaseName(ui->lineEdit_fileLocation->text());

    /* Open database from file full path given in lineEdit field */

    if (teensy_db.open())
    {
        QSqlQuery *query = new QSqlQuery(teensy_db);
        bool ret1 = 0, ret2 = 0;

        ui->label_success->setText("Opened");

        /* Prepare query to display contents of database */
        ret1 = query->prepare(queryString);
        if (ret1 == true)
        {
            /* Execute the query */
            ret2 = query->exec();
            if (ret2 == true)
            {
                /* Load queryModel to display contents in tableView */
                queryModel->setQuery(*query);

                /* close database */
                teensy_db.close();
            }
            else
            {
                ui->label_success->setText("Execute command failed...");
            }
        }
        else
        {
            ui->label_success->setText("Prepare command failed...");
        }

        return (ret1 & ret2);
    }
    else
    {
        ui->label_success->setText("Failed");
        return false;
    }
}

void MainWindow::on_commandLinkButton_go_clicked()
{
    //ui->customPlot->legend->clearItems();
    ui->customPlot->clearGraphs();
    QString queryString = "select * from teensydata order by DeviceID ASC, Timestamp ASC";
    QSqlQueryModel *queryModel = new QSqlQueryModel();

    bool ret = 0;

    ret = queryDatabase(queryModel, queryString);

    if (ret == true)
    {
        /* Display contents in tableView */
        ui->tableView->setModel(queryModel);

        /* Call plotting function */
        makePlot();
    }
    else
    {
        ui->label_success->setText("Failed");
    }
}

void MainWindow::addLine(int index)
{
    ui->customPlot->addGraph(); // line
    ui->customPlot->graph(index)->setPen(QPen(graphColor[index]));
    ui->customPlot->graph(index)->setScatterStyle((QCPScatterStyle::ScatterShape) (index+1));
}

void MainWindow::addDot(int index, int listSize)
{
    ui->customPlot->addGraph(); // dot
    ui->customPlot->graph(index)->setPen(QPen(graphColor[index-listSize]));
    ui->customPlot->graph(index)->setLineStyle(QCPGraph::lsNone);
}

void MainWindow::makePlot()
{
    int row = 0, list = 0;
    QSqlQueryModel *tableModel = new QSqlQueryModel;
    tableModel = (QSqlQueryModel*)ui->tableView->model();

    QStringList deviceID;

    deviceID << tableModel->record(0).value(0).toString();
    for (row = 1; row < tableModel->rowCount(); row++)
    {
        if (tableModel->record(row).value(0).toString() != tableModel->record(row-1).value(0).toString())
        {
            deviceID << tableModel->record(row).value(0).toString();
        }
    }

    int listSize = deviceID.size();

    for (row = 0; row < listSize; row++)
    {
        addLine(row);
    }
//    for (row = listSize; row < 2*listSize; row++)
//    {
//        addDot(row, listSize);
//    }

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
    ui->customPlot->xAxis->setNumberFormat("g");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(1);
    ui->customPlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    QString queryString;
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    double xMax = 0;

    bool ret = 0;

    ui->customPlot->legend->setVisible(false);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica", 9));
    ui->customPlot->legend->setRowSpacing(-3);

    for (list = 0; list < listSize; list++)
    {
        double x = 0;
        double y = 0;
        queryString = "select Data from teensydata where DeviceID=\"" + deviceID[list] + "\" order by Timestamp ASC";
        ret = queryDatabase(queryModel, queryString);

        ui->customPlot->graph(list)->setName(deviceID[list]);
        if (ret == true)
        {
            for (row = 0; row < queryModel->rowCount(); row++)
            {
                y = queryModel->record(row).value(0).toDouble();
                /* Add data to lines*/
                ui->customPlot->graph(list)->addData(x++, y);
                /* Add dots to latest data */
//                ui->customPlot->graph(list + listSize)->clearData();
//                ui->customPlot->graph(list + listSize)->addData(x-1, y);
//                ui->customPlot->legend->removeItem(list + listSize);

                ui->customPlot->graph(list)->rescaleValueAxis(true);
                ui->customPlot->replot();
                if (x > xMax)
                {
                    xMax = x;
                }
            }
        }
    }
    ui->customPlot->xAxis->setRange(xMax-5, 5, Qt::AlignLeft);
}

void MainWindow::on_commandLinkButton_exit_clicked()
{
    teensy_db.close();
    exit(1);
}
