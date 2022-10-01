#ifndef GAME_PROJECT_FLOATINGBLOCKSHOR_H
#define GAME_PROJECT_FLOATINGBLOCKSHOR_H

#include "Object.h"
#include "Wall.h"

class FloatingBlocksHor : public Wall {
    public: int coldDown = 2;
    public: int timeToChangeDirection = -1;
    public: void move() override;
    public: FloatingBlocksHor() = default;
    public: FloatingBlocksHor(int coldDown);
};

void FloatingBlocksHor :: move() {
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown) {
        this->speed.x *= -1;
        this->timeToChangeDirection = -1;
    }

    if (this->speed.x != 0) {
        this->x += this->speed.x;
    }

    this->mapColliderWall = Object ::createRetangleCollider(this->x, this->y, this->z, this->sizeH, this->sizeV);
}

FloatingBlocksHor::FloatingBlocksHor(int coldDown) : Wall(x,  y,  z, mapColliderWall) {
    this->coldDown = coldDown;
}

#endif //GAME_PROJECT_FLOATINGBLOCKSHOR_H
