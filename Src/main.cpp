#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include "myss.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySS* ss = new MySS;
    ss->setPixmap(QPixmap("wallpaper.png"));
    ss->show();
    Widget w;
    QTimer::singleShot(5000,ss,SLOT(close()));
    QTimer::singleShot(5100,&w,SLOT(show()));
    return a.exec();
}
