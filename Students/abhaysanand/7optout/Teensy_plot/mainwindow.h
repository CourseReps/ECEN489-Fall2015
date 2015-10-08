#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtSql>
#include <QFileInfo>
#include <string>
#include <QtCore>
#include <QTimer>
#include <QCoreApplication>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class teensy_thread;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_browse_clicked();

    void on_commandLinkButton_go_clicked();

    void on_commandLinkButton_exit_clicked();

    void makePlot();
    void addDot(int index, int listSize);
    void addLine(int index);
    bool queryDatabase(QSqlQueryModel *queryModel, QString queryString);
    void threadRun();
private:
    Ui::MainWindow *ui;
    QTimer threadTimer;
};

#endif // MAINWINDOW_H
