#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "screenwelcome.h"
#include "screenintro.h"
#include "screenreward.h"
#include "screenobservation.h""
#include "screentrials.h"
#include "screenlander.h"

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


    //Layout Setup
    QVBoxLayout *vLayoutMain;
    vLayoutMain = new QVBoxLayout(this->centralWidget());

    //Tab Setup
    tab = new QTabWidget(this);
   // tab->setStyleSheet("background-color: grey;");
    ui->centralWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    tab->setAutoFillBackground(true);

    //Progres Bar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0,5);

    //Create Screen Widgets
    screenWelcome* welcomeScreen = new screenWelcome(this);
    screenIntro* introScreen = new screenIntro(this);
    screenReward* rewardScreen = new screenReward(this);
    screenObservation* observationScreen = new screenObservation(this);
    screenTrials* trialsScreen = new screenTrials(this);
    screenLander* lunarLanderScreen  = new screenLander(this);

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



    //Add to Main Layout
    vLayoutMain->addWidget(tab);
    vLayoutMain->addWidget(progressBar);

    //Signals and Slots
    connect(tab, &QTabWidget::currentChanged, this, &MainWindow::updateValue);

}

void MainWindow::updateValue(int index){

    int val = progressBar->value();
    switch (index) {
    case 1:
        if(progressBar->value() >= 1){
            break;
        }
        progressBar->setValue(1);
        break;
    case 2:
        if(progressBar->value() >= 2){
            break;
        }
        progressBar->setValue(2);
        break;
    case 3:
        if(progressBar->value() >= 3){
            break;
        }
        progressBar->setValue(3);
        break;
    case 4:
        if(progressBar->value() >= 4){
            break;
        }
        progressBar->setValue(4);
        break;
    case 5:
        if(progressBar->value() >= 5){
            break;
        }
        progressBar->setValue(5);
        break;
    default:
        break;
    }
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


