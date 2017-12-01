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
    //QTabBar* tab = new QTabBar(ui->centralWidget);
    welcomeScreen* screen = new welcomeScreen;
    welcomeScreen* screens = new welcomeScreen;
    QString s = "asdf";

    tab->addTab(screen, s);

    QString st = "OOOOOOOOO";

    tab->addTab(screens, st);
    screen->show();
    screens->show();


}


MainWindow::~MainWindow()
{
    delete ui;
}
