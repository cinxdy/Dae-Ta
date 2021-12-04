#include "thread.h"
#include "./src/Position_moving2.c"
#include <QTextStream>
Thread::Thread(QObject *parent):
    QThread(parent)
{

}

void Thread::run()
{
    system("/home/pi/myQt/Dae-Ta/src/ldown");
    while(true){
        if(m_flag){
            system("/home/pi/myQt/Dae-Ta/src/moving");
            usleep(100000);
            QTextStream(stdout)<<"on";
            r_value = bool_interrupt();
            if(r_value) emit interrupted();
        }

    }
}

//void Thread::movingStart(){
//    while(true){
//        //        Position_moving();
//        system("/home/pi/myQt/Dae-Ta/src/moving");
//        usleep(100000);
//        //        system("/home/pi/myQt/Dae-Ta/src/moving_on");
//        //        qDebug()<<"Thread";
//        //        sleep(1);
//    }
//}
