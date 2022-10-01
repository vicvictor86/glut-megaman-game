#ifndef GAME_PROJECT_WALL_H
#define GAME_PROJECT_WALL_H

#include "Object.h"

class Wall : public Object{
    public: map<char, double> mapColliderWall;
    public: virtual void move();
    public: Wall() = default;
    public: Wall(double x, double y, double z, map<char, double> mapColliderWall);
};

void Wall::move() {}

Wall::Wall(double x, double y, double z, map<char, double> mapColliderWall) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->mapColliderWall = mapColliderWall;
}


#endif //GAME_PROJECT_WALL_H
