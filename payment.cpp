#include "payment.h"
#include "ui_payment.h"

payment::payment(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::payment)
{
    ui->setupUi(this);

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
    QStringList tableMenuRows={"IoT버거","IoT치킨","IoT피자","너가 저항 버거","LED 스파게티","교수님 원픽 김치찌개","콜라","사이다","든든한동"};

    for(int i=0;i<tableMenuRows.length();i++){
        ui->tableMenu->setItem(i,0,new QTableWidgetItem(tableMenuRows[i]));
        ui->tableMenu->setItem(i,1,new QTableWidgetItem(5000));

    }

}

payment::~payment()
{
    delete ui;
}
