#include "welcomescreen.h"
#include "mainwindow.h"
#include "QLabel"
#include <QHBoxLayout>
welcomeScreen::welcomeScreen(QWidget* parent):QMainWindow(parent)
{

    QLabel label;
    label.setText("This is a welcome screen");
    QLayout *layout = new QHBoxLayout();
    layout->addWidget(&label);
    label.show();
    this->setLayout(layout);
}
