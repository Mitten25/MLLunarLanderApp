#ifndef SCREENFUTURE_H
#define SCREENFUTURE_H

#include <QObject>
#include <QWidget>

class screenFuture : public QWidget
{
    Q_OBJECT
public:
    explicit screenFuture(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // SCREENFUTURE_H