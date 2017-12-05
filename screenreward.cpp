#include "screenreward.h"

screenReward::screenReward(QWidget *parent) : QWidget(parent)
{
    headerText = new QLabel(this);
    firstSentence = new QLabel(this);

    font.setBold(true);
    font.setStyleHint(QFont::Helvetica);
    font.setPixelSize(25);

    headerText->setFont(font);
    headerText->setText("Reinforcement Learning");
    headerText->resize(305, 50);
    headerText->show();

    font.setBold(false);
    font.setPixelSize(15);
    firstSentence->setFont(font);
    firstSentence->setText("Reinforcement learning is a subsection of machine learning that \n"
                            "focuses on using a system of rewards to get the AI to learn.");
    firstSentence->resize(500, 100);
    firstSentence->move(0, 25);
    firstSentence->show();

}


screenReward::~screenReward(){
    delete headerText;
}
