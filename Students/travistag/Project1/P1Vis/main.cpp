#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setDBLoc(argv[1]);
    w.show();

    return a.exec();
}
