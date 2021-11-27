#include "home.h"
#include "ui_home.h"

#include <QString>

static QStringList qStrListOptionTable_Headers = {"TableNo","Floor"};

int m_listCount;
int locationX=0;
int locationY=0;
int stateLocation; // 0:home -1:moving n:tableN
Home::Home(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    stateLocation=0;
    if(stateLocation==0) ui->btnOrderOrServe->setText("Serve");
    else ui->btnOrderOrServe->setText("Order");

    ui->tableServingOrder->setRowCount(10);
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

    ui->tableServingOrder->setItem(m_listCount,0,new QTableWidgetItem(QString("Table%1").arg(num)));
    ui->tableServingOrder->setItem(m_listCount,1, new QTableWidgetItem("1"));
    m_listCount++;
    //    QMessageBox msgInputBox;

//    retv=msgInputBox.warning(this, "Confirm","Data Input",QMessageBox::Cancel|QMessageBox::Ok);

}
