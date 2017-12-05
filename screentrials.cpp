#include "screentrials.h"

screenTrials::screenTrials(QWidget *parent) : QWidget(parent)
{
    //Delcare the things
    QVBoxLayout* boxLayout = new QVBoxLayout(this);
    QFont font;
    QLabel *intro = new QLabel(this);


    font.setBold(true);
    font.setStyleHint(QFont::Helvetica);
    font.setPixelSize(100);



}
