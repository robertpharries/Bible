#include "mainwindow.h"
#include "control.h"
#include "bible.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w(0);
//    w.show();

    Control ctrl;
//    ctrl.show();

    return a.exec();
}
