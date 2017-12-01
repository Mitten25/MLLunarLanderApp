#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomescreen.h"
#include <QString>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

  //  QTabWidget* tabb = new QTabWidget(ui->centralWidget);
    QTabBar* tabs = new QTabBar(ui->centralWidget);
    welcomeScreen* screen = new welcomeScreen;
    welcomeScreen* screens = new welcomeScreen;
    tabs->addTab("Welcome");
    tabs->addTab("Machine Learning: An Introduction");
    tabs->addTab("The Reward System");
    tabs->addTab("Lunar Landing Simulation");




}


MainWindow::~MainWindow()
{
    delete ui;
}
