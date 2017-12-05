#ifndef SCREENREWARD_H
#define SCREENREWARD_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>

class screenReward : public QWidget
{
    Q_OBJECT
public:
    explicit screenReward(QWidget *parent = nullptr);

    ~screenReward();

signals:

public slots:

private:
    QGridLayout *layout;
    QLabel *headerText;
    QLabel *firstSentence;
    QLabel *markovText;
    QLabel *rewardText;
    QFont font;


};

#endif // SCREENREWARD_H
