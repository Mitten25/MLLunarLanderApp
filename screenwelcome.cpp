#include "screenwelcome.h"
#include "QLabel"
#include <QHBoxLayout>

screenWelcome::screenWelcome(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(this);
    QFont welcomeFont;

    welcomeFont.setBold(true);
    welcomeFont.setStyleHint(QFont::Helvetica);
    welcomeFont.setPixelSize(100);

    label->setFont(welcomeFont);
    label->setText("Welcome");
    label->resize(1000, 100);
    label->show();








}
