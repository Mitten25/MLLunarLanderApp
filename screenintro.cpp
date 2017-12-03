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
    info1->setText("Machine Learning is the process in which computers apply statistical learning techniques to identify patterns in data. This data can then be used to make highly accurate predictions. \nThis guide will go over Reinforcement Learning, a type of Machine Learning commonly used in games");

    setLayout(boxLayout);
    QPushButton *diagram = new QPushButton;
    QPixmap pixDiagram("../cs3505-f17-a8-edu-app-matwilso/diagrams/rl1png.png");
    QIcon ButtonIcon(pixDiagram);
    diagram->setIcon(ButtonIcon);
    diagram->setIconSize(pixDiagram.rect().size());

    boxLayout->addWidget(intro);
    boxLayout->addWidget(info1);
    boxLayout->addWidget(diagram);
}
