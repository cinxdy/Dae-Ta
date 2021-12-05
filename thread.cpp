#include "thread.h"
// #include "./src/Position_moving2.c"
#include "./src/SW_interrupted.c"
#include "./src/pushedButton.c"
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
//            QTextStream(stdout)<<"on";
            int r_value = bool_interrupt();
//            QTextStream(stdout)<<"SWSWSWWSWSWSW"<<r_value<<endl;
            if(r_value) emit goInterrupted();
        }

        if(battery>75) system("/home/pi/myQt/Dae-Ta/src/full");
        else if(battery>50) system("/home/pi/myQt/Dae-Ta/src/3quarter");
        else if(battery>25) system("/home/pi/myQt/Dae-Ta/src/half");
        else if(battery>10) system("/home/pi/myQt/Dae-Ta/src/quarter");
        else system("/home/pi/myQt/Dae-Ta/src/nothing");

        for(int i=0;i<4;i++){
            if(getOneByteValueOfExe(i)-48==0) emit pushedButton(i+1);
        }

        emit sendMessageSignal();

    }
}

