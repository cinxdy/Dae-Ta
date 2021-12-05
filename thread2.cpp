#include "thread2.h"
// #include "./src/Position_moving2.c"
//#include "./src/SW_interrupted.c"
//#include "./src/pushedButton.c"
//#include "./src/Bell5.c"
#include <QTextStream>

qth::qth(QObject *parent):
    QThread(parent)
{

}

void qth::run()
{
    while(true){
        if(m_flag){
        QMediaPlayer* player =new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("/home/pi/myQt/Dae-Ta/src/moving.mp3"));
        player->setVolume(10);
        player->play();
            usleep(500000);

        }



    }
}

