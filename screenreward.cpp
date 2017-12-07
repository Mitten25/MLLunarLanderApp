#include "screenreward.h"

screenReward::screenReward(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout(this);
    headerText = new QLabel(this);
    firstSentence = new QLabel(this);
    markovText = new QLabel(this);
    rewardText = new QLabel(this);

    font.setBold(true);
    font.setStyleHint(QFont::Helvetica);
    font.setPixelSize(80);

    headerText->setFont(font);
    headerText->setText("Reinforcement Learning");
    headerText->resize(1000, 100);
    headerText->setStyleSheet("QLabel { background-color : black; color : grey; border-color: white; border-style: outset; border-width: 2px; border-color: beige}");

    font.setBold(false);
    font.setPixelSize(15);
    firstSentence->setFont(font);
    firstSentence->setText("Reinforcement learning is a subsection of machine learning that \n"
                            "focuses on using a system of rewards to get the AI to learn.");

    markovText->setFont(font);
    markovText->setText("Reinforcement learning uses something called the Markov Decision Process.\n"
                        "The Markov Decision Process is where you have a series of states that the \n"
                        "Environment and the agent are in, all of the availabe actions that can be taken, \n"
                        "and you have a function called the model that computes the probability of ending\n"
                        "at a select state. Then a reward is given once the agent has taken an action.");

    rewardText->setFont(font);
    rewardText->setText("The agent chooses the action that gives it the most reward. The tricky part \n"
                        "about it is getting the agent to look at long term rewards to best optimize its \n"
                        "solution");


    QPixmap pixReward("../cs3505-f17-a8-edu-app-matwilso/diagrams/reward.png");
    rewardImage = new QLabel(this);
    rewardImage->setPixmap(pixReward);
    rewardImage->setScaledContents(true);
    QPixmap pixMarkov("../cs3505-f17-a8-edu-app-matwilso/diagrams/markov.png");
    markovImage = new QLabel(this);
    markovImage->setPixmap(pixMarkov);
    markovImage->setScaledContents(true);

    layout->addWidget(headerText,0,0,1,0, Qt::AlignHCenter);
    layout->addWidget(firstSentence, 1, 0, Qt::AlignLeft);
    layout->addWidget(markovText, 2, 0, Qt::AlignLeft);
    layout->addWidget(rewardText, 3,0, Qt::AlignLeft);
    layout->addWidget(rewardImage,3,1, Qt::AlignRight);
    layout->addWidget(markovImage, 2,1, Qt::AlignLeft);

}


screenReward::~screenReward(){
    delete headerText;
    delete firstSentence;
    delete markovText;
    delete rewardImage;
    delete markovImage;
}
