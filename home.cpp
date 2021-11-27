#include "home.h"
#include "ui_home.h"

#include <QString>

static QStringList qStrListOptionTable_Headers = {"No","TableNo","Floor"};

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
    ui->tableServingOrder->setColumnCount(3);
    ui->tableServingOrder->setColumnWidth(0,30);
    ui->tableServingOrder->setColumnWidth(1,60);
    ui->tableServingOrder->setColumnWidth(2,60);
    ui->tableServingOrder->setHorizontalHeaderLabels(qStrListOptionTable_Headers);
    ui->tableServingOrder->setStyleSheet("QTableWidget QTableCornerButton::section {"
                               "background-color:rgb(187,187,187);"
                               "border-image: url(:/transparent.png);"
                               "image: url(:/qt-logo.ico);"
                               "}");

    m_listCount=0;
    connect(ui->btnTable1, SIGNAL(clicked()),this, SLOT(addTable()));

}

Home::~Home()
{
    delete ui;
}


void Home::addTable(){

    ui->tableServingOrder->setItem(m_listCount,0,new QTableWidgetItem(QString(QChar(m_listCount))));
    ui->tableServingOrder->setItem(m_listCount,1,new QTableWidgetItem(QString("Table%1").arg(m_listCount)));
    ui->tableServingOrder->setItem(m_listCount,2, new QTableWidgetItem("1"));
    m_listCount++;
    //    QMessageBox msgInputBox;

//    retv=msgInputBox.warning(this, "Confirm","Data Input",QMessageBox::Cancel|QMessageBox::Ok);

}
