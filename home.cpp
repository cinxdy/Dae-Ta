#include "home.h"
#include "ui_home.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QThread>


int m_listCount;
int locationX;
int locationY;
Location stateLocation; // 0:home -1:moving n:tableN
Location destination;

Home::Home(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    locationX=170;
    locationY=200;
    stateLocation=HOME;
    destination=HOME;
    
    if(stateLocation==0) ui->btnOrderOrServe->setText("Serve");
    else ui->btnOrderOrServe->setText("Order");


    // Serving Table
    // ui->tableServingOrder->setRowCount(10);
    ui->tableServingOrder->setColumnCount(2);
    ui->tableServingOrder->setColumnWidth(0,120);
    ui->tableServingOrder->setColumnWidth(1,60);
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

    ui->tableBellOrder->setRowCount(3);
    ui->tableBellOrder->setItem(0,0,new QTableWidgetItem(QString("Table3")));
    ui->tableBellOrder->setItem(1,0,new QTableWidgetItem(QString("Table4")));
    ui->tableBellOrder->setItem(2,0,new QTableWidgetItem(QString("Table6")));
    

    m_listCount=0;
    connect(ui->btnTable1, SIGNAL(clicked()), this, SLOT(addTable1()));
    connect(ui->btnTable2, SIGNAL(clicked()), this, SLOT(addTable2()));
    connect(ui->btnTable3, SIGNAL(clicked()), this, SLOT(addTable3()));
    connect(ui->btnTable4, SIGNAL(clicked()), this, SLOT(addTable4()));
    connect(ui->btnTable5, SIGNAL(clicked()), this, SLOT(addTable5()));
    connect(ui->btnTable6, SIGNAL(clicked()), this, SLOT(addTable6()));
    // connect(this, SIGNAL(locationChanged()),this, SLOT(updateLocation()));

    // Serving Button
    connect(ui->btnOrderOrServe,SIGNAL(clicked()),this,SLOT(servingStart()));
}

Home::~Home()
{
    delete ui;
}

void Home::addTable1(){addTable(1);}
void Home::addTable2(){addTable(2);}
void Home::addTable3(){addTable(3);}
void Home::addTable4(){addTable(4);}
void Home::addTable5(){addTable(5);}
void Home::addTable6(){addTable(6);}

void Home::addTable(int num){
    ui->tableServingOrder->insertRow(m_listCount);
    ui->tableServingOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(num)));
    ui->tableServingOrder->setItem(m_listCount,1, new QTableWidgetItem("1"));
    m_listCount++;
    //    QMessageBox msgInputBox;

//    retv=msgInputBox.warning(this, "Confirm","Data Input",QMessageBox::Cancel|QMessageBox::Ok);

}

void Home::servingStart(){

    while(ui->tableServingOrder->rowCount()!=0){
        if(ui->tableServingOrder->item(0,0)->text()=="Table1") destination=TABLE1;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table2") destination=TABLE2;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table3") destination=TABLE3;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table4") destination=TABLE4;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table5") destination=TABLE5;
        else if(ui->tableServingOrder->item(0,0)->text()=="Table6") destination=TABLE6;

        
        stateLocation=MOVING; // moving state
        QTextStream(stdout)<<destination;

        int mvResult = goToTable(destination);
        if(mvResult) {
            stateLocation = destination;
            
            // Hello Message
            do{
                QMessageBox msgConfirmBox;
                int retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\n Did you get your plates?").arg(destination), QMessageBox::Cancel|QMessageBox::Ok);
                if(retv==QMessageBox::Ok) break;
            }
            while(true);

            // QMessageBox msgConfirmBox;
            // int retv=msgConfirmBox.warning(this, "Confirm",QString("Table%1!!\n Do you want to order something?").arg(destination), QMessageBox::Cancel|QMessageBox::Ok);
            // if(retv==QMessageBox::Ok) break;
            // else

        }
    
        ui->tableServingOrder->removeRow(0);
        m_listCount--;
    }

    while(ui->tableBellOrder->rowCount()!=0){
        if(ui->tableBellOrder->item(0,0)->text()=="Table1") destination=TABLE1;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table2") destination=TABLE2;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table3") destination=TABLE3;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table4") destination=TABLE4;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table5") destination=TABLE5;
        else if(ui->tableBellOrder->item(0,0)->text()=="Table6") destination=TABLE6;

        
        stateLocation=MOVING; // moving state
        QTextStream(stdout)<<destination;

        int mvResult = goToTable(destination);
        if(mvResult) {
            stateLocation = destination;
            
            // Hello Message
        QMessageBox msgConfirmBox;
        int retv=msgConfirmBox.warning(this, "Confirm",QString("Hello, Table%1!!\n If you want to order, press Order button").arg(destination), QMessageBox::Cancel|QMessageBox::Ok);

        }
    
        ui->tableBellOrder->removeRow(0);
    }

    goToTable(HOME);
}

int Home::goToTable(Location dest){
    // location
    // kitchen 170,200
    // table1 340,140
    // table2 515,140
    // table3 690,140
    // table4 340,250
    // table5 515,250
    // table6 690,250

    LocationXY* destTable[7];
    destTable[0] = new LocationXY{170,200}; // Kitchen
    destTable[1] = new LocationXY{340,140};
    destTable[2] = new LocationXY{515,140};
    destTable[3] = new LocationXY{690,140};
    destTable[4] = new LocationXY{340,250};
    destTable[5] = new LocationXY{515,250};
    destTable[6] = new LocationXY{690,250};

    while(true){
        if(locationX==destTable[dest]->x) break;
        else if(locationX<destTable[dest]->x) locationX++;
        else locationX--;
        updateLocation();
    }
    while(true){
        if(locationY==destTable[dest]->y) break;
        else if(locationY<destTable[dest]->y) locationY++;
        else locationY--;
        updateLocation();
    }

    stateLocation=dest;
    return 1;
}


void Home:: updateLocation(){
    ui->lbLocation->move(locationX,locationY);
    ui->lbLocation->repaint();
    QThread::usleep(10000);
}