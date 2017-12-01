#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomescreen.h"
#include <QString>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTabWidget* tab = new QTabWidget(ui->centralWidget);
    welcomeScreen* new_welcomescreen = new welcomeScreen;
    welcomeScreen* introScreen = new welcomeScreen;
    welcomeScreen* rewardScreen = new welcomeScreen;
    welcomeScreen* lunarLanderScreen  = new welcomeScreen;
    QString s = "Welcome";
    QString st = "Machine Learning: An Introduction";
    QString rewardString = "The Reward System";
    QString lunarString = "Lunar Landing Simulation";
    tab->addTab(new_welcomescreen, s);
    tab->addTab(introScreen, st);
    tab->addTab(rewardScreen, rewardString);
    tab->addTab(lunarLanderScreen, lunarString);
    new_welcomescreen->show();
    introScreen->show();
    rewardScreen->show();
    lunarLanderScreen->show();


}


MainWindow::~MainWindow()
{
    delete ui;
}
