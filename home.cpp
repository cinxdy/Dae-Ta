#include "home.h"
#include "ui_home.h"
#include "payment.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QTextStream>

static int m_listCount;
static int locationX;
static int locationY;
static int interrupted;
static int gotFood;
static Location stateLocation; // 0:home -1:moving n:tableN
static Location destination;

Home::Home(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    t=new Thread(this);
    t->m_flag=0;
    connect(t, SIGNAL(goInterrupted()), this, SLOT(interruptMoving()));
    t->start();

    system("echo 0 > /sys/class/gpio/export");
    usleep(1000);
    system("echo 1 > /sys/class/gpio/export");
    usleep(1000);
    system("echo 4 > /sys/class/gpio/export");
    usleep(1000);
    system("echo 5 > /sys/class/gpio/export");
    usleep(1000);
    system("echo in > /sys/class/gpio/gpio0/direction");
    usleep(1000);
    system("echo in > /sys/class/gpio/gpio1/direction");
    usleep(1000);
    system("echo in > /sys/class/gpio/gpio4/direction");
    usleep(1000);
    system("echo in > /sys/class/gpio/gpio5/direction");
    usleep(1000);

    inputTimer = new QTimer(this); // Read from Dev
    inputTimer->start(500);

    connect(inputTimer, SIGNAL(timeout()),this, SLOT(tableBellOrder()));

    locationX=170;
    locationY=170;
    stateLocation=HOME;
    t->battery=100;
    ui->lbstateLocation->setText("HOME");

    destination=HOME;
    system("/home/pi/myQt/Dae-Ta/src/ldown");

    if(stateLocation==0) ui->btnOrderOrServe->setText("Serve");
    else ui->btnOrderOrServe->setText("Order");


    // Serving Table
    // ui->tableServingOrder->setRowCount(10);
    ui->tableServingOrder->setColumnCount(2);
    ui->tableServingOrder->setColumnWidth(0,120);
    ui->tableServingOrder->setColumnWidth(1,80);
    ui->tableServingOrder->setHorizontalHeaderLabels({"TableNo","Floor"});
    ui->tableServingOrder->setStyleSheet("QTableWidget QTableCornerButton::section {"
                                         "background-color:rgb(187,187,187);"
                                         "border-image: url(:/transparent.png);"
                                         "image: url(:/qt-logo.ico);"
                                         "}");


    ui->tableBellOrder->setColumnCount(1);
    ui->tableBellOrder->setColumnWidth(0,200);
    ui->tableBellOrder->setHorizontalHeaderLabels({"Bell Order"});
    ui->tableBellOrder->setStyleSheet("QTableWidget QTableCornerButton::section {"
                                      "background-color:rgb(187,187,187);"
                                      "border-image: url(:/transparent.png);"
                                      "image: url(:/qt-logo.ico);"
                                      "}");

    ui->lbLocation->setStyleSheet("QLabel {"
                                  "border-color: rgb(66, 69, 183);"
                                  "border-width: 3px;"
                                  "border-style: solid;"
                                  "border-radius: 30px;"
                                  "margin:10px;"
                                  // "padding:10px;"
                                  "}");

    ui->tableBellOrder->setRowCount(3);
    ui->tableBellOrder->setItem(0,0,new QTableWidgetItem(QString("Table3")));
    ui->tableBellOrder->setItem(1,0,new QTableWidgetItem(QString("Table4")));
    ui->tableBellOrder->setItem(2,0,new QTableWidgetItem(QString("Table5")));


    m_listCount=0;
    connect(ui->btnTable1, SIGNAL(clicked()), this, SLOT(addTable1()));
    connect(ui->btnTable2, SIGNAL(clicked()), this, SLOT(addTable2()));
    connect(ui->btnTable3, SIGNAL(clicked()), this, SLOT(addTable3()));
    connect(ui->btnTable4, SIGNAL(clicked()), this, SLOT(addTable4()));
    connect(ui->btnTable5, SIGNAL(clicked()), this, SLOT(addTable5()));/*
    connect(t, SIGNAL(goInterrupted()), this, SLOT(interruptMoving()));*/


    // Serving Button
    connect(ui->btnOrderOrServe,SIGNAL(clicked()),this,SLOT(btnOrderOrServeClicked()));


}


void Home::btnOrderOrServeClicked(){
//    QMediaPlayer* player =  new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
//        QFile file("/home/pi/myQt/Dae-Ta/src/letsgo.wav");
//        file.open(QIODevice::ReadOnly);
//        QByteArray *ba = new QByteArray();
//        ba->append(file.readAll());
//        QBuffer *buffer = new QBuffer(ba);
//        buffer->open(QIODevice::ReadOnly);
//        buffer->reset();  // same as buffer.seek(0);
//        qDebug() << "Buffer size:" << buffer->size(); // is the file loaded
//        player->setMedia(QMediaContent(), buffer);
//        player->play();
//    QMediaPlayer* m_media =new QMediaPlayer;
//    m_media->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/letsgo.wav"));
//    m_media->setVolume(50);
//    m_media->play();
    sleep(1);
    if(stateLocation==HOME) servingStart();
    else if(stateLocation==MOVING) interruptMoving();
    else openPayment();

}

Home::~Home()
{
    system("echo 0 > /sys/class/gpio/unexport");
    system("echo 1 > /sys/class/gpio/unexport");
    system("echo 4 > /sys/class/gpio/unexport");
    system("echo 5 > /sys/class/gpio/unexport");
    delete ui;
}

void Home::addTable1(){addTable(1);}
void Home::addTable2(){addTable(2);}
void Home::addTable3(){addTable(3);}
void Home::addTable4(){addTable(4);}
void Home::addTable5(){addTable(5);}
// void Home::addTable6(){addTable(6);}

void Home::addTable(int num){

    QMessageBox msgInputBox;
    int floor=msgInputBox.warning(this, "Confirm","Which floor did you put the food?","First Floor","Second Floor","Third Floor")+1;
    QTextStream(stdout)<< floor;

    ui->tableServingOrder->insertRow(m_listCount);
    ui->tableServingOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(num)));
    ui->tableServingOrder->setItem(m_listCount,1, new QTableWidgetItem(QString("%1").arg(floor)));
    m_listCount++;


}

void Home::servingStart(){
    QMediaPlayer* player =  new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
        QFile file("/home/pi/myQt/Dae-Ta/src/letsgo.wav");
        file.open(QIODevice::ReadOnly);
        QByteArray *ba = new QByteArray();
        ba->append(file.readAll());
        QBuffer *buffer = new QBuffer(ba);
        buffer->open(QIODevice::ReadOnly);
        buffer->reset();  // same as buffer.seek(0);
        qDebug() << "Buffer size:" << buffer->size(); // is the file loaded
        player->setMedia(QMediaContent(), buffer);
        player->play();
    QMessageBox msgConfirmBox;

    while(ui->tableServingOrder->rowCount()!=0){
        if(ui->tableServingOrder->item(0,0)->text()=="Table1") destination=TABLE1;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table2") destination=TABLE2;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table3") destination=TABLE3;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table4") destination=TABLE4;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table5") destination=TABLE5;
        // else if(ui->tableServingOrder->item(0,0)->text()=="Table6") destination=TABLE6;

        QTextStream(stdout)<<destination;

        int mvResult = goToTable(destination);
        if(mvResult) {
            t->battery-=10;
            // Hello Message

            int retv;
            while(true){
                // Speaker Guide
                retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\nDid you get your plates?").arg(destination), "No", "Yes");
                if(retv) break;
            }

            retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\nDo you want to order something?").arg(destination), "No", "Yes");
            if(retv) {
                for(int i=0;i<ui->tableBellOrder->rowCount();i++){
                    QTextStream(stdout)<<ui->tableBellOrder->item(i,0)->text()<<endl;
                    QTextStream(stdout)<<QString("Table%1").arg(destination)<<endl;

                    //                    if(ui->tableBellOrder->item(i,0)->text()==QString("Table%1").arg(destination))
                    //                        ui->tableBellOrder->removeRow(i);
                }
                openPayment();
                QEventLoop loop;
                connect(this, SIGNAL(restart()), &loop, SLOT(quit()));
                loop.exec();
            }

            ui->tableServingOrder->removeRow(0);
            m_listCount--;
        }
        else{
            int retv=msgConfirmBox.warning(this, "Confirm",QString("Do you want to go again?"), "No", "Yes");
        }

    }


    goToTable(HOME);


}

void Home::goToBellTable(){
    while(ui->tableBellOrder->rowCount()!=0){
        if(ui->tableBellOrder->item(0,0)->text()=="Table1") destination=TABLE1;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table2") destination=TABLE2;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table3") destination=TABLE3;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table4") destination=TABLE4;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table5") destination=TABLE5;
        //        else if(ui->tableBellOrder->item(0,0)->text()=="Table6") destination=TABLE6;

        ui->tableBellOrder->removeRow(0);

        QTextStream(stdout)<<destination;
        int mvResult;
        while(true){
            mvResult = goToTable(destination);
            if(mvResult) {
                // Hello Message
                QMessageBox msgConfirmBox;
                int retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\n If you want to order, press Order button").arg(destination), QMessageBox::Cancel|QMessageBox::Ok);
                if(retv==QMessageBox::Ok) break;
            }
            else if(interrupted){
                interrupted=false;
                break;
            }

            while(gotFood){
                //            sleep(1);
                ;
            }

        }

        goToTable(HOME);
    }
}

int Home::goToTable(Location dest){
    // location
    // kitchen 170,170
    // table1 340,120
    // table2 515,120
    // table3 690,120
    // table4 340,220
    // table5 515,220
    // table6 690,220

    LocationXY* destTable[7];
    destTable[0] = new LocationXY{170,170}; // Kitchen
    destTable[1] = new LocationXY{340,120};
    destTable[2] = new LocationXY{515,120};
    destTable[3] = new LocationXY{690,120};
    destTable[4] = new LocationXY{340,220};
    destTable[5] = new LocationXY{602,220};
    //    destTable[6] = new LocationXY{690,220};

    stateLocation=MOVING;
    ui->lbstateLocation->setText("MOVING");
    system("/home/pi/myQt/Dae-Ta/src/mmoving");

    //    std::thread t(Position_moving);
    //    t.join();

    //    thread->start();
    t->m_flag=1;

    while(true){
        if(interrupted) {
            stateLocation=INTERRUPTED;
            interrupted=0;
            return 0;
        }
        if(locationX==destTable[dest]->x) break;
        else if(locationX<destTable[dest]->x) locationX++;
        else locationX--;

        ui->lbLocation->move(locationX,locationY);
        ui->lbLocation->repaint();
        //        system("/home/pi/myQt/Dae-Ta/src/moving_on");
    }

    while(true){
        if(interrupted==1) {
            stateLocation=INTERRUPTED;
            interrupted=0;
            break;
            return 0;
        }
        if(interrupted==0) {
        if(locationY==destTable[dest]->y) break;
        else if(locationY<destTable[dest]->y) locationY++;
        else locationY--;
        }


        ui->lbLocation->move(locationX,locationY);
        ui->lbLocation->repaint();
        //        system("/home/pi/myQt/Dae-Ta/src/moving_on");
    }

    t->m_flag=0;
    stateLocation=dest;

    if(!stateLocation) {
        ui->lbstateLocation->setText("HOME");
        system("/home/pi/myQt/Dae-Ta/src/home");
    }
    else {
        ui->lbstateLocation->setText(QString("TABLE%1").arg(stateLocation));
        system("/home/pi/myQt/Dae-Ta/src/table");
    }

    system("/home/pi/myQt/Dae-Ta/src/stopped");
    return 1;

}
void Home::tableBellOrder(){

    if(getOneByteValueOfExe(0)-48==0){
        ui->tableBellOrder->insertRow(m_listCount);
        ui->tableBellOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(1)));
        m_listCount++;
    }

    if(getOneByteValueOfExe(1)-48==0){
        ui->tableBellOrder->insertRow(m_listCount);
        ui->tableBellOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(2)));
        m_listCount++;
    }

    if(getOneByteValueOfExe(2)-48==0){
        ui->tableBellOrder->insertRow(m_listCount);
        ui->tableBellOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(3)));
        m_listCount++;
    }

    if(getOneByteValueOfExe(3)-48==0){
        ui->tableBellOrder->insertRow(m_listCount);
        ui->tableBellOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(4)));
        m_listCount++;
    }
}

void Home::openPayment(){
    p=new payment();
    p->show();
    this->hide();

    connect(p, SIGNAL(closePayment()), this, SLOT(openHomeAgain()));
}

void Home::openHomeAgain(){
    p->hide();
    delete p;
    this->show();
    sleep(1);
    emit restart();
}


void Home::interruptMoving(){
    interrupted=1;
    QTextStream(stdout)<<"EndEndEnd";

}

unsigned char Home::getOneByteValueOfExe(int chan)
{

    FILE *pFile;
    unsigned char value;

    switch(chan)
    {
    case 0:
        pFile = popen("cat /sys/class/gpio/gpio0/value", "r");
        break;
    case 1:
        pFile = popen("cat /sys/class/gpio/gpio1/value", "r");
        break;
    case 2:
        pFile = popen("cat /sys/class/gpio/gpio4/value", "r");
        break;
    case 3:
        pFile = popen("cat /sys/class/gpio/gpio5/value", "r");
        break;
    default:
        qDebug() << "xxx";
        break;
    }


    value = fgetc(pFile);


    pclose(pFile);

    return value;
}
