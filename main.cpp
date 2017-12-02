#include "mainwindow.h"
#include <QApplication>
#include "game.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
   // game();

    return a.exec();
}
