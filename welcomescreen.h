#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QtWidgets>

class welcomeScreen : public QWidget
{
    Q_OBJECT
public:
  explicit  welcomeScreen(QWidget* parent = 0);

};

#endif // WELCOMESCREEN_H
