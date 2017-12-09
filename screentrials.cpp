#include "screentrials.h"

screenTrials::screenTrials(QWidget *parent) : QWidget(parent)
{
    QLabel *intro = new QLabel(this);
  //  QVBoxLayout *boxLayout = new QVBoxLayout;


    QGridLayout* girdLayout = new QGridLayout(this);

    intro->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");
    QFont tabFont;
    QComboBox *choices = new QComboBox;

    tabFont.setBold(true);
    tabFont.setStyleHint(QFont::Helvetica);
    tabFont.setPixelSize(100);

    intro->setFont(tabFont);
    intro->setText("Trials");
    intro->resize(1000, 100);

    choices->addItem("100 Trials", QString::number(0));
    choices->addItem("1000 Trials", QString::number(1));
    choices->addItem("10000 Trials", QString::number(2));

    QLabel *info1 = new QLabel(this);
    QString text = "In reinforcement learning, the more trials you run, the more information that the model has to predict actions.";
    text+= "\n This means that the more trials you have the more accurate the model can be";
    /*
    text+= "This data can then be used to make highly accurate predictions.";
    text+= "\nThis guide will go over Reinforcement Learning, a type of Machine Learning commonly used in games";
    text+= "\n In reinforcement learning you have the agent do some action in the environment, and based on the action you give it rewards ";
    text+= "\n ";
    */

    info1->setText(text);


    QLabel* gifSpot = new QLabel(this);
    gifSpot->setText("LASDJFLASKDJFASDLKFJASLDKFJASLDKJFASLDKJF");

    //Adding to the layout
    girdLayout->addWidget(intro,   0,0,1,0);
    girdLayout->addWidget(info1,   1,0,1,1);
    girdLayout->addWidget(gifSpot, 1,1,1,1);
    girdLayout->addWidget(choices, 2,0,1,2);




}
