#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include "thread.h"
#include "thread2.h"
#include "payment.h"
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QTextStream>
#include <QThread>
#include <QTcpSocket>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "thread.h"
#include "server.h"
#include "payment.h"
#include "stateEnum.h"


namespace Ui {
class Home;
}

class Home : public QMainWindow
{

    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

    int m_listCount;
    int b_listCount;
    int locationX;
    int locationY;
    int interrupted;
    int sleep_value;
    Location stateLocation; // 0:home -1:moving n:tableN
    Location destination;

    QMediaPlayer* player;
    QTimer* inputTimer;
    unsigned char getOneByteValueOfExe(int chan);

public slots:
    void addTable(int);
    void btnOrderOrServeClicked();
    void goToBellTable();
    void openPayment();
    void updateMessage();

    void servingStart();
    int goToTable(Location);
    void goToHome();
    void addBellTable(int);
    void openHomeAgain();
    void addTable1();
    void addTable2();
    void addTable3();
    void addTable4();
    void addTable5();
    void interruptMoving();
    void changeV(int );

    void timerSet();


signals:
    void stateLocationChanged();
    void restart();
    void goToBellTableSignal();
    void interruptMoving_sig();
    void messageSendSignal();
    void goAgain();
    void goHome();

private:
    Ui::Home *ui;
    Thread *t;
    Thread2 *t2;
    payment *p;
    server *s;
};

#endif // HOME_H
