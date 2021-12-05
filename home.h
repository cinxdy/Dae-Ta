#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
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
    Location stateLocation; // 0:home -1:moving n:tableN
    Location destination;

    QMediaPlayer* player;
    QTimer* inputTimer;
    unsigned char getOneByteValueOfExe(int chan);
    void interruptMoving();
    void addTable1();
    void addTable2();
    void addTable3();
    void addTable4();
    void addTable5();
    
public slots:
    void addTable(int);
    void btnOrderOrServeClicked();
    void goToBellTable();
    void openPayment();
    void updateMessage();
    void servingStart();
    int goToTable(Location);
    void addBellTable(int);
    void openHomeAgain();

signals:
    void stateLocationChanged();
    void restart();
    void goToBellTableSignal();
    void messageSendSignal();

private:
    Ui::Home *ui;
    Thread* t;
    payment *p;
    server *s;
};

#endif // HOME_H
