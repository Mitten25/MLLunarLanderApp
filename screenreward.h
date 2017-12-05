#ifndef SCREENREWARD_H
#define SCREENREWARD_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class screenReward : public QWidget
{
    Q_OBJECT
public:
    explicit screenReward(QWidget *parent = nullptr);

    ~screenReward();

signals:

public slots:

private:
    QLabel *headerText;
    QLabel *firstSentence;
    QFont font;

};

#endif // SCREENREWARD_H
