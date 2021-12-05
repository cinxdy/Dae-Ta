#ifndef THREAD_H
#define THREAD_H

#include<QThread>
#include<QDebug>

class Thread : public QThread
{
    Q_OBJECT
public:
    int m_flag;
    int battery;
   explicit Thread(QObject* parent =0);


//public slots:
//    void movingStart();

signals:
    void goInterrupted();
    void closePayment();
    void pushedButton(int);
    void sendMessageSignal();

private:
    void run();




};

#endif // THREAD_H
