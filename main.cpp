#include "home.h"
#include "payment.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Home w;
//    w.show();

    payment p;
    p.show();

    return a.exec();
}
