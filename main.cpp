#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argc, argv);
    w.show();
    int i = a.exec();
    return i;
}
