#include "home.h"
#include "ui_home.h"

#include <QString>
#include <QMessageBox>
#include<QDebug>


static QStringList qStrListOptionTable_Headers = {"TableNo","Floor"};

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
    ui->tableServingOrder->setHorizontalHeaderLabels(qStrListOptionTable_Headers);
    ui->tableServingOrder->setStyleSheet("QTableWidget QTableCornerButton::section {"
                               "background-color:rgb(187,187,187);"
                               "border-image: url(:/transparent.png);"
                               "image: url(:/qt-logo.ico);"
                               "}");

    m_listCount=0;
    connect(ui->btnTable1, SIGNAL(clicked()), this, SLOT(addTable1()));
    connect(ui->btnTable2, SIGNAL(clicked()), this, SLOT(addTable2()));
    connect(ui->btnTable3, SIGNAL(clicked()), this, SLOT(addTable3()));
    connect(ui->btnTable4, SIGNAL(clicked()), this, SLOT(addTable4()));
    connect(ui->btnTable5, SIGNAL(clicked()), this, SLOT(addTable5()));
    connect(ui->btnTable6, SIGNAL(clicked()), this, SLOT(addTable6()));

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

            do{
                int retv=msgClearBox.warning(this, "Confirm","Did you get your plates?", QMessageBox::Cancel|QMessageBox::Ok);
                if(retv==QMessageBox::Ok) break;
            while(true);

        }
        

        ui->tableServingOrder->removeRow(0);
        m_listCount--;
    }
}

int Home:goToTable(Location dest){
    
    
    return 1;
}