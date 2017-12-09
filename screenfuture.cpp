#include "screenfuture.h"

screenFuture::screenFuture(QWidget *parent) : QWidget(parent)
{
    //Declare the stuff
    QLabel *intro = new QLabel(this);
    QGridLayout* gridLayout = new QGridLayout(this);
    QLabel *info1 = new QLabel(this);
    QFont textFont;
    QFont tabFont;
    gif = new QLabel;
    QPushButton* playBot = new QPushButton(this);
    QPushButton* playGif = new QPushButton(this);

    //Button Basics
    playBot->setText("Start Bot");
    playGif->setText("Start Gif");

    //Intro stylesheet and setup
    intro->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");
    tabFont.setBold(true);
    tabFont.setStyleHint(QFont::Helvetica);
    tabFont.setPixelSize(100);
    intro->setFont(tabFont);
    intro->setText("Past Vs. Future");
    intro->resize(1000, 100);

    //Tab body
    textFont.setPixelSize(20);
    QString text = "In Reinforcement learning, the model \n";
    text+= "needs to be able to observe the evironment";
    text+= "\nThe more it can observe, the better";
    text+= "\nWe can see what happens when we blind";
    text+= "\nthe model in certain ways";
    text+= "";
    info1->setFont(textFont);
    info1->setText(text);

    //Adding to the layout
    gridLayout->addWidget(intro,   0,0,1,0);
    gridLayout->addWidget(info1,   1,0,1,1);
    gridLayout->addWidget(gif, 1,1,1,1);
    gridLayout->addWidget(playBot, 2,0);
    gridLayout->addWidget(playGif, 2,1);

    connect(playBot, SIGNAL(clicked(bool)), this, SLOT(playBot()));
    connect(playGif, SIGNAL(clicked(bool)), this, SLOT(playGif()));

}

void screenFuture::playBot(){
    //true is bot, false is player
    extern int game(bool);
    QFuture<void> future = QtConcurrent::run(game, true);
}

void screenFuture::playGif(){
    QMovie *movie = new QMovie("../cs3505-f17-a8-edu-app-matwilso/diagrams/obsANGLE-2.gif");
    gif->setMovie(movie);
    movie->setSpeed(200);
    movie->start();
}
