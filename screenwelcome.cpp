#include "screenwelcome.h"
#include "QLabel"
#include <QHBoxLayout>

screenWelcome::screenWelcome(QWidget *parent) : QWidget(parent)
{

    QGridLayout *layout = new QGridLayout;


    QLabel *label = new QLabel(this);
    QLabel *labell = new QLabel(this);

    QFont welcomeFont;

    layout->addWidget(label);
    layout->addWidget(labell);

    welcomeFont.setBold(true);
    welcomeFont.setStyleHint(QFont::Helvetica);
    welcomeFont.setPixelSize(100);

    labell->setText("setset");
    labell->setFont(welcomeFont);
    //layout->

    label->setAlignment(Qt::AlignCenter);
    label->setFont(welcomeFont);
    label->setText("Welcome");
    label->resize(1000, 100);
   // label->show();








}
