#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcomescreen.h"
#include <QString>
#include <QSizePolicy>
#include <QVBoxLayout>
#include<QLabel>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *vLayoutMain;

    vLayoutMain = new QVBoxLayout(this->centralWidget());


    QTabWidget* tab = new QTabWidget(ui->centralWidget);
    ui->centralWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    //Create Screen Widgets
    QWidget* welcomescreen = new QWidget();
    QWidget* introScreen = new QWidget();
    QWidget* rewardScreen = new QWidget();
    QWidget* lunarLanderScreen  = new QWidget();

    //Create Tab Names
    QString welcome = "Welcome";
    QString intro = "Machine Learning: An Introduction";
    QString rewardString = "The Reward System";
    QString lunarString = "Lunar Landing Simulation";

    //Add Widgets/.CPP to tab screens
    tab->addTab(welcomescreen, welcome);
    tab->addTab(introScreen, intro);
    tab->addTab(rewardScreen, rewardString);
    tab->addTab(lunarLanderScreen, lunarString);

    //Display Screens
    welcomescreen->show();
    introScreen->show();
    rewardScreen->show();
    lunarLanderScreen->show();
    vLayoutMain->addWidget(tab);

    //Welcome screen text
    QLabel *label = new QLabel(welcomescreen);
    QFont welcomeFont;

    welcomeFont.setBold(true);
    welcomeFont.setStyleHint(QFont::Helvetica);
    welcomeFont.setPixelSize(100);

    label->setFont(welcomeFont);
    label->setText("Welcome");
    label->resize(1000, 100);
    label->show();
}


MainWindow::~MainWindow()
{
    delete ui;
//    delete tab;
//    delete welcomescreen;
//    delete introScreen;
//    delete rewardScreen;
//    delete lunarLanderScreen;
}
