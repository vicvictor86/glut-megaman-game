#ifndef GAME_PROJECT_FLOATINGBLOCKS_H
#define GAME_PROJECT_FLOATINGBLOCKS_H

#include "Object.h"

class FloatingBlocks : public Object {
    public: int coldDown = 2;
    public: int timeToChangeDirection = -1;
    public: void move();
    public: FloatingBlocks() = default;
    public: Object wallObject;
    public: map<char, double> mapColliderWall;
};

void FloatingBlocks :: move() {
    cout << "speed" << this->speed.y << endl;
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown) {
        this->speed.y *= -1;
        this->timeToChangeDirection = -1;
    }

    if (this->speed.y != 0) {
        this->y += this->speed.y;
    }
}

#endif //GAME_PROJECT_FLOATINGBLOCKS_H
