#ifndef THREAD2_H
#define THREAD2_H

#include<QThread>
#include<QDebug>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

class Thread2: public QThread
{
    Q_OBJECT
public:
    int m_flag;
    explicit Thread2(QObject *parent =nullptr);
private:
    void run();

};

#endif // THREAD2_H
