#include "screenobservation.h"

screenObservation::screenObservation(QWidget *parent) : QWidget(parent)
{
    QLabel *intro = new QLabel(this);
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
    /*
    text+= "This data can then be used to make highly accurate predictions.";
    text+= "\nThis guide will go over Reinforcement Learning, a type of Machine Learning commonly used in games";
    text+= "\n In reinforcement learning you have the agent do some action in the environment, and based on the action you give it rewards ";
    text+= "\n ";
    */

    info1->setText(text);

    setLayout(boxLayout);


    //Adding to the layout
    boxLayout->addWidget(intro);
    boxLayout->addWidget(info1);
    boxLayout->addWidget(choices);
}
