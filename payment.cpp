#include "payment.h"
#include "ui_payment.h"
#include <QMessageBox>

static QStringList tableMenuRows={"IoT버거","IoT치킨","IoT피자","너가 저항 버거","LED 스파게티","교수님 원픽 김치찌개","콜라","사이다","든든한동","싸이버거"};
static QStringList tableMenuPrices={"5,000원","10,000원","10,000원","100,000원","10,000원","500원","1,000원","1,000원","2,800원","3,900원"};

static int priceList[]={5000,10000,10000,100000,10000, 500,1000,1000,2800,3900};
payment::payment(QWidget *parent) :
   QWidget(parent),
    ui(new Ui::payment)
{
    ui->setupUi(this);
    m_listCount=0;
//    system("/home/pi/myQt/Dae-Ta/src/order");
    ui->tableMenu->setRowCount(10);
    ui->tableMenu->setColumnCount(2);
    ui->tableMenu->setColumnWidth(0,120);
    ui->tableMenu->setColumnWidth(1,80);
    ui->tableMenu->setHorizontalHeaderLabels({"Menu","Price"});
    ui->tableMenu->setStyleSheet("QTableWidget QTableCornerButton::section {"
                               "background-color:rgb(187,187,187);"
                               "border-image: url(:/transparent.png);"
                               "image: url(:/qt-logo.ico);"
                               "}");
                               

//    ui->tableMenu->setRowCount(10);
    ui->tableCart->setColumnCount(3);

    ui->tableCart->setColumnHidden(2, true);
    ui->tableCart->setColumnWidth(0,120);
    ui->tableCart->setColumnWidth(1,80);
    ui->tableCart->setHorizontalHeaderLabels({"Menu","Price"});
    ui->tableCart->setStyleSheet("QTableWidget QTableCornerButton::section {"
                               "background-color:rgb(187,187,187);"
                               "border-image: url(:/transparent.png);"
                               "image: url(:/qt-logo.ico);"
                               "}");

    for(int i=0;i<tableMenuRows.length();i++){
        ui->tableMenu->setItem(i,0,new QTableWidgetItem(tableMenuRows[i]));
        ui->tableMenu->setItem(i,1,new QTableWidgetItem(tableMenuPrices[i]));
        
    }
    connect(ui->tableMenu,SIGNAL(cellClicked(int,int)),this,SLOT(addGetItem(int,int)));
    connect(ui->tableCart,SIGNAL(cellClicked(int,int)),this,SLOT(deleteItem(int,int)));
    connect(ui->btnPay, SIGNAL(clicked()),this, SLOT(btnPayClicked()));
 
}

void payment::addGetItem(int row, int ){
    ui->tableCart->insertRow(m_listCount);
    ui->tableCart->setItem(m_listCount,0,new QTableWidgetItem(tableMenuRows[row]));
    ui->tableCart->setItem(m_listCount,1, new QTableWidgetItem(tableMenuPrices[row]));
    ui->tableCart->setItem(m_listCount,2, new QTableWidgetItem(QString("%1").arg(row)));
    m_listCount++;

    updateSum();
}
void payment::deleteItem(int row, int ){
    ui->tableCart->removeRow(row);
    m_listCount--;

    updateSum();
}

void payment::updateSum(){
    int sum=0;
    for(int i=0;i<ui->tableCart->rowCount();i++){
        sum+= priceList[ui->tableCart->item(i,2)->text().toInt()];
    }
    ui->lbPrice->setText(QString("%1").arg(sum));
}

void payment::btnPayClicked(){
    QMessageBox msgConfirmBox;
    int retv=msgConfirmBox.warning(this, "Confirm",QString("Total price is %1 Korean Won.\nDo you want to pay and close the order page?").arg(ui->lbPrice->text()), "No","Yes");
    pay=1;
    system("/home/pi/myQt/Dae-Ta/src/succeed");
    if(retv) emit closePayment();
}

payment::~payment()
{
    delete ui;
}
