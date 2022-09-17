#include "speed.h"
#include "Collision.h"
#include <map>
using namespace std; 

#ifndef OBJECT_H
#define OBJECT_H

class Object {
    public: double x, y, z;
    public: float r, g, b;
    public: Speed speed;
    public: float size;
    public: Collision collision;
    public: static void drawnWall(double x, double y, double z, double size);
    public: static map<char, double> createRetangleCollider(double x, double y, double z, double size);
    public: Object(){};
    public: Object(double x, double y, double z, float r, float g, float b, Speed speed, float size, Collision collision);
};

Object :: Object(double x, double y, double z, float r, float g, float b, Speed speed, float size, Collision collision){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->speed = Speed(speed.x, speed.y, speed.z);
    this->size = size;
    this->collision = collision;
};

map<char, double> Object:: createRetangleCollider(double x, double y, double z, double size){
    map<char, double> mapColliders;
    mapColliders.insert(pair<char, double>('L', x - (size / 2)));
    mapColliders.insert(pair<char, double>('R', x + (size / 2)));
    mapColliders.insert(pair<char, double>('T', y + (size / 2)));
    mapColliders.insert(pair<char, double>('B', y - (size / 2)));

    return mapColliders;
}

void Object:: drawnWall(double x, double y, double z, double size){
    glPushMatrix();
        glTranslated(x, y, z);
        glRotated(90, 1, 0, 0);
        glutWireCube(size);
    glPopMatrix();
}

#endif