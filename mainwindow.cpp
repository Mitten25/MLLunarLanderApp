#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "screenwelcome.h"
#include "screenintro.h"
#include "screenreward.h"
#include "screenobservation.h""
#include "screentrials.h"

#include <QString>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QLabel>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //setup for the mainwindow
    ui->setupUi(this);
    QSize* minSize = new QSize(1100, 600);
    this->setMinimumSize(*minSize);
    this->setMaximumSize(*minSize);
    this->setWindowTitle("MoonMoon");
//    this->setStyleSheet("background-color: grey;");
//    ui->centralWidget->setStyleSheet("background-color: grey;");



    QVBoxLayout *vLayoutMain;
    vLayoutMain = new QVBoxLayout(this->centralWidget());


    QTabWidget* tab = new QTabWidget(ui->centralWidget);
   // tab->setStyleSheet("background-color: grey;");
    ui->centralWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    //Create Screen Widgets
    screenWelcome* welcomeScreen = new screenWelcome(this);
    screenIntro* introScreen = new screenIntro();
    screenReward* rewardScreen = new screenReward();
    screenObservation* observationScreen = new screenObservation();
    screenTrials* trialsScreen = new screenTrials();

    QWidget* lunarLanderScreen  = new QWidget();

    //Create Tab Names
    QString welcome = "Welcome";
    QString intro = "Machine Learning: An Introduction";
    QString rewardString = "The Reward System";
    QString lunarString = "Lunar Landing Simulation";
    QString observationString = "Observation";
    QString trialsString = "Trials";

    //Add Widgets/.CPP to tab screens
    tab->addTab(welcomeScreen, welcome);
    tab->addTab(introScreen, intro);
    tab->addTab(rewardScreen, rewardString);
    tab->addTab(observationScreen, observationString);
    tab->addTab(trialsScreen, trialsString);
    tab->addTab(lunarLanderScreen, lunarString);

    //Display Screens
   // welcomescreen->show();
   // introScreen->show();
//    rewardScreen->show();
//    lunarLanderScreen->show();
    vLayoutMain->addWidget(tab);

    //Welcome screen text
//    QLabel *label = new QLabel(welcomescreen);
//    QFont welcomeFont;

//    welcomeFont.setBold(true);
//    welcomeFont.setStyleHint(QFont::Helvetica);
//    welcomeFont.setPixelSize(100);

//    label->setFont(welcomeFont);
//    label->setText("Welcome");
//    label->resize(1000, 100);
//    label->show();
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
