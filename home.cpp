#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::Home)
{
    // Thread setup
    t = new Thread(this);
    t2= new Thread2(this);

    t->m_flag=0;
    t2->m_flag=0;

    t->r_value=0;
    connect(t, SIGNAL(goInterrupted()), this, SLOT(interruptMoving()));
    
    t->start();
    t2->start();

    t->battery = 100;


    inputTimer = new QTimer(this); // Read from Dev
    inputTimer->start(500);

       connect(inputTimer, SIGNAL(timeout()),this, SLOT(updateMessage()));
    // Server setup
    s = new server();
//    connect(t, SIGNAL(messageSendSignal()), this, SLOT(updateMessage()));
    connect(this, SIGNAL(messageSendSignal()), s, SLOT(sendMessage()));
    connect(s, SIGNAL(changeVSignal(int)),this, SLOT(changeV(int)));

    // Device setup
    system("sudo echo 0 > /sys/class/gpio/export");
    usleep(1000);
    system("sudo echo 1 > /sys/class/gpio/export");
    usleep(1000);
    system("sudo echo 4 > /sys/class/gpio/export");
    usleep(1000);
    system("sudo echo 5 > /sys/class/gpio/export");
    usleep(1000);
    system("sudo echo in > /sys/class/gpio/gpio0/direction");
    usleep(1000);
    system("sudo echo in > /sys/class/gpio/gpio1/direction");
    usleep(1000);
    system("sudo echo in > /sys/class/gpio/gpio4/direction");
    usleep(1000);
    system("sudo echo in > /sys/class/gpio/gpio5/direction");
    usleep(1000);


    connect(t, SIGNAL(pushedButton(int)), this, SLOT(addBellTable(int)));
    connect(this, SIGNAL(goToBellTableSignal()),this,SLOT(goToBellTable()));

    // UI setup
    ui->setupUi(this);
    sleep_value=1;
    locationX = 170;
    locationY = 170;
    stateLocation = HOME;
    ui->lbstateLocation->setText("HOME");

    destination = HOME;
    system("/home/pi/myQt/Dae-Ta/src/ldown");

    if (stateLocation == 0)
        ui->btnOrderOrServe->setText("Serve");
    else
        ui->btnOrderOrServe->setText("Order");

    // Serving Table
    // ui->tableServingOrder->setRowCount(10);
    ui->tableServingOrder->setColumnCount(2);
    ui->tableServingOrder->setColumnWidth(0, 120);
    ui->tableServingOrder->setColumnWidth(1, 80);
    ui->tableServingOrder->setHorizontalHeaderLabels({"TableNo", "Floor"});


    // Bell table
    ui->tableBellOrder->setColumnCount(1);
    ui->tableBellOrder->setColumnWidth(0, 200);
    ui->tableBellOrder->setHorizontalHeaderLabels({"Bell Order"});


    m_listCount = 0;
    b_listCount = 0;
    connect(ui->btnTable1, SIGNAL(clicked()), this, SLOT(addTable1()));
    connect(ui->btnTable2, SIGNAL(clicked()), this, SLOT(addTable2()));
    connect(ui->btnTable3, SIGNAL(clicked()), this, SLOT(addTable3()));
    connect(ui->btnTable4, SIGNAL(clicked()), this, SLOT(addTable4()));
    connect(ui->btnTable5, SIGNAL(clicked()), this, SLOT(addTable5()));
    connect(this, SIGNAL(interruptMoving_sig()), this, SLOT(interruptMoving()));


    // Serving Button
    connect(ui->btnOrderOrServe, SIGNAL(clicked()), this, SLOT(btnOrderOrServeClicked()));
}


void Home::btnOrderOrServeClicked(){
    if(stateLocation==HOME) {
        QMediaPlayer* player =new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/go.wav"));
        player->setVolume(50);
        player->play();
        servingStart();
    }
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

void Home::addTable1() { addTable(1); }
void Home::addTable2() { addTable(2); }
void Home::addTable3() { addTable(3); }
void Home::addTable4() { addTable(4); }
void Home::addTable5() { addTable(5); }

void Home::addTable(int num)
{
    QMessageBox msgInputBox;
    int floor = msgInputBox.warning(this, "Confirm", "Which floor did you put the food?", "First Floor", "Second Floor", "Third Floor") + 1;
    QTextStream(stdout) << floor;

    ui->tableServingOrder->insertRow(m_listCount);
    ui->tableServingOrder->setItem(m_listCount, 0, new QTableWidgetItem(QString("Table%1").arg(num)));
    ui->tableServingOrder->setItem(m_listCount, 1, new QTableWidgetItem(QString("%1").arg(floor)));
    m_listCount++;
}

void Home::servingStart(){

    //    QMediaPlayer* player =  new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    //    QFile file("/home/pi/myQt/Dae-Ta/src/letsgo.wav");
    //    file.open(QIODevice::ReadOnly);
    //    QByteArray *ba = new QByteArray();
    //    ba->append(file.readAll());
    //    QBuffer *buffer = new QBuffer(ba);
    //    buffer->open(QIODevice::ReadOnly);
    //    buffer->reset();  // same as buffer.seek(0);
    //    qDebug() << "Buffer size:" << buffer->size(); // is the file loaded
    //    player->setMedia(QMediaContent(), buffer);
    //    player->play();


    QMessageBox msgConfirmBox;

    while (ui->tableServingOrder->rowCount() != 0)
    {
        if (ui->tableServingOrder->item(0, 0)->text() == "Table1")
            destination = TABLE1;
        else if (ui->tableServingOrder->item(0, 0)->text() == "Table2")
            destination = TABLE2;
        else if (ui->tableServingOrder->item(0, 0)->text() == "Table3")
            destination = TABLE3;
        else if (ui->tableServingOrder->item(0, 0)->text() == "Table4")
            destination = TABLE4;
        else if (ui->tableServingOrder->item(0, 0)->text() == "Table5")
            destination = TABLE5;

        QTextStream(stdout) << destination;

        int mvResult = goToTable(destination);
        if (mvResult)
        {
            t->battery -= 10;
            // Hello Message

            int retv;
            while (true)
            {
                // Speaker Guide
                QMediaPlayer* player =new QMediaPlayer;
                player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/hi.wav"));
                player->setVolume(30);
                player->play();

                player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/food.wav"));
                player->setVolume(30);
                player->play();

                retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\nDid you get your plates?").arg(destination), "No", "Yes");
                if(retv){
                    player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/eat.wav"));
                    player->setVolume(30);
                    player->play();

                    break;
                }
            }

            retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\nDo you want to order something?").arg(destination), "No", "Yes");
            if(retv) {
                for(int i=0;i<ui->tableBellOrder->rowCount();i++){
                    //                    QTextStream(stdout)<<ui->tableBellOrder->item(i,0)->text()<<endl;
                    //                    QTextStream(stdout)<<QString("Table%1").arg(destination)<<endl;

                    if (ui->tableBellOrder->item(i, 0)->text().split("e")[1].toInt() == destination)
                        ui->tableBellOrder->removeRow(i);
                }

                openPayment();
            }

            ui->tableServingOrder->removeRow(0);
            m_listCount--;
        }
        else
        {
            int retv = msgConfirmBox.warning(this, "Confirm", QString("Do you want to go again?"), "No", "Yes");
        }
    }

    emit goToBellTableSignal();
}

void Home::goToBellTable()
{
    while (ui->tableBellOrder->rowCount() != 0)
    {
        if (ui->tableBellOrder->item(0, 0)->text() == "Table1")
            destination = TABLE1;
        else if (ui->tableBellOrder->item(0, 0)->text() == "Table2")
            destination = TABLE2;
        else if (ui->tableBellOrder->item(0, 0)->text() == "Table3")
            destination = TABLE3;
        else if (ui->tableBellOrder->item(0, 0)->text() == "Table4")
            destination = TABLE4;
        else if (ui->tableBellOrder->item(0, 0)->text() == "Table5")
            destination = TABLE5;
        
        ui->tableBellOrder->removeRow(0);

        QTextStream(stdout) << destination;
        QMessageBox msgConfirmBox;
        int mvResult = goToTable(destination);
        if (mvResult)
        {
            t->battery -= 10;
            // Hello Message
            QMediaPlayer* player =new QMediaPlayer;
            player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/arrived.wav"));
            player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/hi.wav"));
            player->setVolume(30);
            player->play();
            int retv;

            retv = msgConfirmBox.warning(this, "Confirm", QString("Hello, Table%1!!\nDo you want to order something?").arg(destination), "No", "Yes");
            if (retv)
            {
                openPayment();
            }

            ui->tableBellOrder->removeRow(0);
            b_listCount--;
        }
        else
        {
            int retv = msgConfirmBox.warning(this, "Confirm", QString("Do you want to go again?"), "No", "Yes");
        }
    }

    goToTable(HOME);
}

int Home::goToTable(Location dest)
{
    // location
    // kitchen 170,170
    // table1 340,120
    // table2 515,120
    // table3 690,120
    // table4 340,220
    // table5 515,220
    // table6 690,220

    LocationXY *destTable[7];
    destTable[0] = new LocationXY{100,170}; // Kitchen
    destTable[1] = new LocationXY{160,80};
    destTable[2] = new LocationXY{310,80};
    destTable[3] = new LocationXY{440,80};
    destTable[4] = new LocationXY{180,250};
    destTable[5] = new LocationXY{100,170};

    stateLocation = MOVING;
    ui->lbstateLocation->setText("MOVING");
    system("/home/pi/myQt/Dae-Ta/src/mmoving");

    t->m_flag=1;
    t2->m_flag=1;

    while(true){
        if(t->r_value) {
            stateLocation=INTERRUPTED;
            emit interruptMoving_sig();
            //                    interrupted=0;
            return 0;
        }
        if(locationX==destTable[dest]->x) break;
        else if(locationX<destTable[dest]->x) locationX++;
        else locationX--;
        ui->lbLocation->move(locationX,locationY);
        ui->lbLocation->repaint();
        usleep(sleep_value);
    }

    while(true){
        if(t->r_value) {
            stateLocation=INTERRUPTED;
            emit interruptMoving_sig();
            //                    interrupted=0;
            //                    break;
            return 0;
        }
        if(locationY==destTable[dest]->y) break;
        else if(locationY<destTable[dest]->y) locationY++;
        else locationY--;
        ui->lbLocation->move(locationX,locationY);
        ui->lbLocation->repaint();
        usleep(sleep_value);
    }

    t->m_flag=0;
    t2->m_flag=0;
    stateLocation=dest;

    if (!stateLocation)
    {
        ui->lbstateLocation->setText("HOME");
        system("/home/pi/myQt/Dae-Ta/src/home");
    }
    else
    {
        ui->lbstateLocation->setText(QString("TABLE%1").arg(stateLocation));
        system("/home/pi/myQt/Dae-Ta/src/table");
    }

    system("/home/pi/myQt/Dae-Ta/src/stopped");
    return 1;
}
void Home::addBellTable(int tableNo)
{
    int exist = false;
    for (int j = 0; j < ui->tableBellOrder->rowCount(); j++)
    {
        if (tableNo == ui->tableBellOrder->item(j, 0)->text().split("e")[1].toInt())
            exist = true;
    }

    if (!exist)
    {
        ui->tableBellOrder->insertRow(b_listCount);
        ui->tableBellOrder->setItem(b_listCount, 0, new QTableWidgetItem(QString("Table%1").arg(tableNo)));
        b_listCount++;
    }

    if (stateLocation == HOME)
        emit goToBellTableSignal();
}

void Home::openPayment()
{
    p = new payment();
    p->show();
//    this->hide();

    QEventLoop loop;
    connect(p, SIGNAL(closePayment()), this, SLOT(openHomeAgain()));
    connect(this, SIGNAL(restart()), &loop, SLOT(quit()));
    loop.exec();
}

void Home::openHomeAgain()
{
    p->hide();
    delete p;
//    this->show();
    
    sleep(1);
    emit restart();
}

void Home::interruptMoving()
{
    ui->lbstateLocation->setText("INTERRUPTED");
    interrupted = 1;
}

void Home::updateMessage()
{
//        s->message->stateLocation=stateLocation;
//    emit messageSendSignal();
}


void Home::changeV(int v){
    int s=100000;
    for(int i=0;i<v;i++)
        s/=10;
    
    sleep_value=s;
}


