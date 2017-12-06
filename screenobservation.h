#ifndef SCREENOBSERVATION_H
#define SCREENOBSERVATION_H

#include <QObject>
#include <QWidget>
#include "QLabel"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QComboBox>

class screenObservation : public QWidget
{
    Q_OBJECT
public:
    explicit screenObservation(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // SCREENOBSERVATION_H
