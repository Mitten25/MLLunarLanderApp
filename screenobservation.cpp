#include "screenobservation.h"

screenObservation::screenObservation(QWidget *parent) : QWidget(parent)
{
    QLabel *intro = new QLabel(this);
    gif = new QLabel;
    QGridLayout* girdLayout = new QGridLayout(this);
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

    //Adding to the layout
    girdLayout->addWidget(intro,   0,0,1,0);
    girdLayout->addWidget(info1,   1,0,1,1);
    girdLayout->addWidget(gif, 1,1,1,1);
    girdLayout->addWidget(choices, 2,0,1,2);

    connect(choices, SIGNAL(activated(int)), this, SLOT(change(int)));
}

void screenObservation::change(int index) {
    QMovie *movie = new QMovie("../cs3505-f17-a8-edu-app-matwilso/diagrams/trials100.gif");
    gif->setMovie(movie);
    movie->setSpeed(200);
    movie->start();
}





