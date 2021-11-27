#ifndef HOME_H
#define HOME_H

#include <QMainWindow>

namespace Ui {
class Home;
}

class Home : public QMainWindow
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

public slots:
    void addTable(int);
    void addTable1();
    void addTable2();
    void addTable3();
    void addTable4();
    void addTable5();
    void addTable6();

private:
    Ui::Home *ui;
};

#endif // HOME_H
