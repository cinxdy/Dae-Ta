#include "home.h"
#include <QCoreApplication>
#include <QThread>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Home w;
    w.show();

    return a.exec();
}
