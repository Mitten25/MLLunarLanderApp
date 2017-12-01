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
    welcomeScreen* screen = new welcomeScreen;
    QString s = "asdf";
    //tab->addTab(screen, s);


    //ui->centralWidget->addAction(tab);

}


MainWindow::~MainWindow()
{
    delete ui;
}
