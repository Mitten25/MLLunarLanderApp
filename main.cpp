#include "mainwindow.h"
#include <QApplication>
//#include "game.cpp"
#include "learn_game.cpp"
#include <QThread>
#include <QFuture>
#include <QtConcurrent>


int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    //Comment these out to stop the game
    extern int learn_game();
    learn_game();

    //QFuture<void> future = QtConcurrent::run(learn_game);


    //return a.exec();
    return 0;
}
