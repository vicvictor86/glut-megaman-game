#ifndef GAME_PROJECT_FLOATINGBLOCKS_H
#define GAME_PROJECT_FLOATINGBLOCKS_H

#include "Object.h"
#include "Wall.h"

class FloatingBlocks : public Wall {
    public: int coldDown = 2;
    public: int timeToChangeDirection = -1;
    public: void move() override;
    public: FloatingBlocks() = default;
    public: FloatingBlocks(int coldDown);
};

void FloatingBlocks :: move() {
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

    this->mapColliderWall = Object ::createRetangleCollider(this->x, this->y, this->z, this->sizeH, this->sizeV);
}

FloatingBlocks::FloatingBlocks(int coldDown) : Wall(x,  y,  z, mapColliderWall) {
    this->coldDown = coldDown;
}

#endif //GAME_PROJECT_FLOATINGBLOCKS_H
