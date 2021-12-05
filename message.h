#ifndef MESSAGE_H
#define MESSAGE_H

#include "stateEnum.h"
#include <QString>

class Message{
public:
    Location stateLocation;
    QString textmsg;
    int bettery;
    int velocity;
};

#endif // MESSAGE_H
