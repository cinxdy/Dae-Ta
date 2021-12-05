#include "home.h"
#include <QApplication>
#include <QThread>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Home w;
//    w.show();

    QDesktopWidget widget;
    QRect screenGeometry = widget.screenGeometry();

    int height = screenGeometry.height();
    int width = screenGeometry.width();

    //set to middle of screen
    w.move((width - w.width()) / 2.0,
                    (height - w.height()) / 2.0);
    w.show();
    return a.exec();
}
