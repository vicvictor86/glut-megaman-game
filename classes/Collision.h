#include <map>
using namespace std;

#ifndef COLLISION_H
#define COLLISION_H

class Collision {
    public: double x, y, z;
    public: float r, g, b;
    public: float size;
    public: bool isOnPlataform = false;
    public: Collision(){};
    public: Collision(double x, double y, double z, float r, float g, float b, float size);
};

Collision :: Collision(double x, double y, double z, float r, float g, float b, float size){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->size = size;
};

#endif