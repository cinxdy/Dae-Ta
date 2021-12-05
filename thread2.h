#ifndef qth_H
#define qth_H

#include<QThread>
#include<QDebug>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

class qth: public QThread
{
    Q_OBJECT
public:
    int m_flag;
   explicit qth(QObject *parent =nullptr);


//public slots:
//    void movingStart();

//signals:



private:
    void run();



};

#endif // qth_H
