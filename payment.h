#ifndef PAYMENT_H
#define PAYMENT_H

#include <QMainWindow>

namespace Ui {
class payment;
}

class payment : public QMainWindow
{
    Q_OBJECT

public:
    explicit payment(QWidget *parent = nullptr);
    ~payment();

private:
    Ui::payment *ui;
};

#endif // PAYMENT_H
