#ifndef GAME_PROJECT_FLOATINGBLOCKS_H
#define GAME_PROJECT_FLOATINGBLOCKS_H

#include "Object.h"
#include "Wall.h"

class FloatingBlocks : public Wall {
    public: int coldDown = 2;
    public: int timeToChangeDirection = -1;
    public: void move() override;
    public: FloatingBlocks() = default;
    public: FloatingBlocks(Object wallObject, map<char, double> mapColliderWall, int coldDown);
};

void FloatingBlocks :: move() {
    cout << "TANKA AI" << endl;
    if (this->timeToChangeDirection == -1) {
        this->timeToChangeDirection = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToChangeDirection > this->coldDown) {
        this->speed.y *= -1;
        this->timeToChangeDirection = -1;
    }

    if (this->speed.y != 0) {
        this->wallObject.y += this->speed.y;
    }

    this->mapColliderWall = Object ::createRetangleCollider(this->wallObject.x, this->wallObject.y, this->wallObject.z, this->sizeH, this->sizeV);
}

FloatingBlocks::FloatingBlocks(Object wallObject, map<char, double> mapColliderWall, int coldDown=2) {
    this->wallObject = wallObject;
    this->mapColliderWall = mapColliderWall;
    this->coldDown = coldDown;
}

#endif //GAME_PROJECT_FLOATINGBLOCKS_H
