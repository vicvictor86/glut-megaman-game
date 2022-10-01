#ifndef GAME_PROJECT_WALL_H
#define GAME_PROJECT_WALL_H

#include "Object.h"

class Wall : public Object{
    public: Object wallObject;
    public: map<char, double> mapColliderWall;
    public: virtual void move();
    public: Wall() = default;
    public: Wall(Object wallObject, map<char, double> mapColliderWall);
};

void Wall::move() {}

Wall::Wall(Object wallObject, map<char, double> mapColliderWall) {
    this->wallObject = wallObject;
    this->mapColliderWall = mapColliderWall;
}


#endif //GAME_PROJECT_WALL_H
