#ifndef ENUM_H
#define ENUM_H

enum Location {MOVING=-1,HOME, TABLE1,TABLE2,TABLE3,TABLE4,TABLE5,INTERRUPTED}; // 0:home -1:moving n:tableN
class LocationXY {
    public:
        int x;
        int y;
};

#endif
