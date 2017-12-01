#include "welcomescreen.h"
#include "mainwindow.h"
#include "QLabel"
#include <QHBoxLayout>
welcomeScreen::welcomeScreen(QWidget* parent):QMainWindow(parent)
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
