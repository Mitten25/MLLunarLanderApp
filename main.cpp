#include "mainwindow.h"
#include <QApplication>
#include "game.cpp"
#include <QThread>
#include <QFuture>
#include <QtConcurrent>
#include "rl/reinforce.h"


int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    ////Comment these out to stop the game
    //extern int game();
    //QFuture<void> future = QtConcurrent::run(game);


    //return a.exec();
    REINFORCE reinforce(8, 4);
    std::vector<float> obs;
    obs.push_back(1.f);
    obs.push_back(1.f);
    obs.push_back(1.f);
    obs.push_back(1.f);
    obs.push_back(1.f);
    obs.push_back(1.f);
    obs.push_back(1.f);
    obs.push_back(1.f);
    std::cout << "sample: " << reinforce.selectAction(obs) << std::endl;

    return 0;
}
