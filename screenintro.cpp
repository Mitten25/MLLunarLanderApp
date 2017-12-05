#include "screenintro.h"

screenIntro::screenIntro(QWidget *parent) : QWidget(parent)
{
    QLabel *intro = new QLabel(this);
    QVBoxLayout *boxLayout = new QVBoxLayout;
    intro->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");
    QFont tabFont;

    tabFont.setBold(true);
    tabFont.setStyleHint(QFont::Helvetica);
    tabFont.setPixelSize(100);

    intro->setFont(tabFont);
    intro->setText("Machine Learning");
    intro->resize(1000, 100);


    QLabel *info1 = new QLabel(this);
    QString text = "Machine Learning is the process in which computers apply statistical learning techniques to identify patterns in data. ";
    text+= "This data can then be used to make highly accurate predictions.";
    text+= "\nThis guide will go over Reinforcement Learning, a type of Machine Learning commonly used in games";
    text+= "\n In reinforcement learning you have the agent do some action in the environment, and based on the action you give it rewards ";
    text+= "\n ";

    info1->setText(text);

    setLayout(boxLayout);
    diagram = new QPushButton;
    QPixmap pixDiagram1("../cs3505-f17-a8-edu-app-matwilso/diagrams/rl1png.png");
    QIcon ButtonIcon1(pixDiagram1);
    diagram->setIcon(ButtonIcon1);
    diagram->setIconSize(pixDiagram1.rect().size());
    flag = 0;


    //Adding to the layout
    boxLayout->addWidget(intro);
    boxLayout->addWidget(info1);
    boxLayout->addWidget(diagram);

    connect(diagram, SIGNAL(clicked(bool)), this, SLOT(change(bool)));
}

void screenIntro::change(bool) {
    if(flag == 0) {
        QPixmap pixDiagram2("../cs3505-f17-a8-edu-app-matwilso/diagrams/progeny.png");
        QIcon ButtonIcon2(pixDiagram2);
        diagram->setIcon(ButtonIcon2);
        flag = 1;
    }
    else {
        QPixmap pixDiagram1("../cs3505-f17-a8-edu-app-matwilso/diagrams/rl1png.png");
        QIcon ButtonIcon1(pixDiagram1);
        diagram->setIcon(ButtonIcon1);
        diagram->setIconSize(pixDiagram1.rect().size());
        flag = 0;
    }
}

