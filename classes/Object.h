#include "speed.h"
#ifndef OBJECT_H
#define OBJECT_H

class Object {
    public: double x, y, z;
    public: float r, g, b;
    public: Speed speed;
    public: float radius;
    public: Object(){};
    public: Object(double x, double y, double z, float r, float g, float b, Speed speed, float radius);
};

Object :: Object(double x, double y, double z, float r, float g, float b, Speed speed, float radius){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->speed = Speed(speed.x, speed.y, speed.z);
    this->radius = radius;
};

#endif