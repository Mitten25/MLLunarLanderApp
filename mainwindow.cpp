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
    QTabBar* tab = new QTabBar(ui->centralWidget);
    welcomeScreen* screen = new welcomeScreen;
    welcomeScreen* screens = new welcomeScreen;

    QString s = "asdf";

    tab->addTab( s);

    QString st = "OOOOOOOOO";

    tab->addTab( st);



}


MainWindow::~MainWindow()
{
    delete ui;
}
