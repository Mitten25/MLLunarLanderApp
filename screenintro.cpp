#include "screenintro.h"

screenIntro::screenIntro(QWidget *parent) : QWidget(parent)
{
    QLabel *intro = new QLabel(this);
    QVBoxLayout *boxLayout = new QVBoxLayout;
    intro->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");
    QFont tabFont;

    tabFont.setBold(true);
    tabFont.setStyleHint(QFont::Helvetica);
    tabFont.setPixelSize(70);

    intro->setFont(tabFont);
    intro->setText("What is Machine Learning?");
    intro->resize(1000, 100);

    QFont textFont;
    textFont.setStyleHint(QFont::Helvetica);
    textFont.setPixelSize(20);
    QLabel *info1 = new QLabel(this);
    info1->setFont(textFont);
    QString text = "Machine Learning is the process in which computers apply statistical learning techniques to identify patterns in data. \n";
    text+= "This data can then be used to make a model, which can then make highly accurate predictions\n";
    text+= "Machine learning is a rapidly expanding field that is employed in a wide variety of computing tasks\n";
    text+= "\nThis guide will go over Reinforcement Learning, a type of Machine Learning commonly used in games\n";
    text+= "Reinforcement learning is a subsection of machine learning that focses on using a system of rewards to get the AI to learn";


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

