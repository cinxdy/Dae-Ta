#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

unsigned char getOneByteValueOfExe(int chan)
{

    FILE *pFile;
    unsigned char value;

    switch(chan)
    {
    case 0:
        pFile = popen("cat /sys/class/gpio/gpio0/value", "r");
        break;
    case 1:
        pFile = popen("cat /sys/class/gpio/gpio1/value", "r");
        break;
    case 2:
        pFile = popen("cat /sys/class/gpio/gpio4/value", "r");
        break;
    case 3:
        pFile = popen("cat /sys/class/gpio/gpio5/value", "r");
        break;
    default:
//        qDebug() << "xxx";
        break;
    }


    value = fgetc(pFile);


    pclose(pFile);

    return value;
}
