#ifndef MESSAGE_H
#define MESSAGE_H

#include "stateEnum.h"
#include <QString>

class Message{
public:
    Location stateLocation;
    int bettery;
    int velocity;
    int moving;
    int card;
    int work;
};

#endif // MESSAGE_H
