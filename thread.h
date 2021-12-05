#ifndef THREAD_H
#define THREAD_H

#include<QThread>
#include<QDebug>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

class Thread : public QThread
{
    Q_OBJECT
public:
//    QMediaPlayer* player;
    int m_flag;
    int r_value;
    int bettery;
    int B_value;
    int w_flag;


   explicit Thread(QObject *parent =nullptr);


//public slots:
//    void movingStart();

signals:
    void goInterrupted();
    void closePayment();
    void pushedButton(int);
    void messageSendSignal();


private:
    void run();




};

#endif // THREAD_H
