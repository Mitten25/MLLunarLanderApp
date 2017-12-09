#include "screenlander.h"
#include <QFuture>
#include <QtConcurrent>
screenLander::screenLander(QWidget *parent) : QWidget(parent)
{
     layout = new QGridLayout(this);
     titleLabel = new QLabel(this);
     playGameLabel = new QLabel(this);
     playGameButton = new QPushButton(this);

     font.setBold(true);
     font.setStyleHint(QFont::Helvetica);
     font.setPixelSize(80);

     titleLabel->setFont(font);
     titleLabel->setText("Lunar Lander Game");
     titleLabel->resize(1000, 100);
     titleLabel->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");

     font.setPixelSize(30);
     playGameLabel->setFont(font);
     playGameLabel->setText("Maybe you can take what you have learned about how machinces learn to get better at \n"
                             "Lunar Lander and see if you can use the same tactics yourself");
     playGameLabel->resize(300, 100);

     playGameButton->setText("Play Game");

     layout->addWidget(titleLabel, 0, 0, Qt::AlignCenter);
     layout->addWidget(playGameLabel, 1, 0, Qt::AlignCenter);
     layout->addWidget(playGameButton, 2, 0, Qt::AlignCenter);

     connect(playGameButton, SIGNAL(clicked(bool)), this, SLOT(onPress()));
}

screenLander::~screenLander(){
    delete layout;
    delete titleLabel;
    delete playGameLabel;
    delete playGameButton;
}


void screenLander::onPress(){
    extern int game(bool);
    QFuture<void> future = QtConcurrent::run(game, false);
}
