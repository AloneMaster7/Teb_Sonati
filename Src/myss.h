#ifndef MYSS_H
#define MYSS_H

#include <QObject>
#include <QSplashScreen>
#include <QMouseEvent>

class MySS : public QSplashScreen
{
    Q_OBJECT
public:
    MySS();
protected slots:
    void mousePressEvent(QMouseEvent *) override {} //by default, it close after clicking on
    //splash screen
};

#endif // MYSS_H
