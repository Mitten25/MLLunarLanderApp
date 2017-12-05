#include "mainwindow.h"
#include <QApplication>
#include "game.cpp"
#include <QThread>
#include <QFuture>
#include <QtConcurrent>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

 //   game();
    //extern int game();
    //QFuture<void> future = QtConcurrent::run(game);


    return a.exec();
}
