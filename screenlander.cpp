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
     playGameLabel->setText("Here you can try the game for yourself." \
                            "\nOur hard coded bot got a high score of around 72" \
                            "\nOur ML bot got an average score of 193" \
                            "\nSee how you can do!");
     playGameLabel->resize(300, 100);

     playGameButton->setText("Play Game");
     playGameButton->setStyleSheet("background-color: rgb(53, 50, 47)");

     layout->addWidget(titleLabel, 0, 0);
     layout->addWidget(playGameLabel, 1, 0, Qt::AlignCenter);
     layout->addWidget(playGameButton, 2, 0);

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
