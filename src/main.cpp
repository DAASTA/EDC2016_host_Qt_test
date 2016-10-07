#include "mainwindow.h"
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
	Widget w;
    w.show();

    return a.exec();
}
