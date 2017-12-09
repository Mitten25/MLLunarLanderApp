#include "screenobservation.h"

screenObservation::screenObservation(QWidget *parent) : QWidget(parent)
{
    QLabel *intro = new QLabel(this);
    gif = new QLabel;
    QVBoxLayout *boxLayout = new QVBoxLayout;
    intro->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");
    QFont tabFont;
    QComboBox *choices = new QComboBox;

    tabFont.setBold(true);
    tabFont.setStyleHint(QFont::Helvetica);
    tabFont.setPixelSize(100);

    intro->setFont(tabFont);
    intro->setText("Observations");
    intro->resize(1000, 100);

    choices->addItem("Not knowing Y Position", QString::number(0));
    choices->addItem("Not knowing X Postiion", QString::number(1));
    choices->addItem("Not knowing Angle", QString::number(2));

    QLabel *info1 = new QLabel(this);
    QString text = "Reinforcement learning requires observation of the environment to work properly";


    info1->setText(text);

    setLayout(boxLayout);



    //Adding to the layout
    boxLayout->addWidget(intro);
    boxLayout->addWidget(info1);
    boxLayout->addWidget(choices);
    boxLayout->addWidget(gif);

    connect(choices, SIGNAL(activated(int)), this, SLOT(change(int)));
}

void screenObservation::change(int index) {
    QMovie *movie = new QMovie("../cs3505-f17-a8-edu-app-matwilso/diagrams/trials100.gif");
    gif->setMovie(movie);
    movie->setSpeed(200);
    movie->start();
}





