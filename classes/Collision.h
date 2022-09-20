#include <map>
using namespace std;

#ifndef COLLISION_H
#define COLLISION_H

enum collisionDirections {TOPCOLLISION, BOTTOMCOLLISION, LEFTCOLLISION, RIGHTCOLLISION, NOCOLLISION, NULLCOLLISION};

class Collision {
    public: double x, y, z;
    public: float r, g, b;
    public: float size;
    public: bool isOnPlataform = false;
    public: Collision(){};
    public: Collision(double x, double y, double z, float r, float g, float b, float size);
    public: static collisionDirections checkCollision(map<char, double> mapCollider1, double x1, double y1, map<char, double> mapCollider2, bool lastIteration, int* quantityOverLapping);
};

Collision :: Collision(double x, double y, double z, float r, float g, float b, float size){
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
    this->size = size;
}

collisionDirections Collision :: checkCollision(map<char, double> mapCollider1, double x1, double y1, map<char, double> mapCollider2, bool lastIteration, int* quantityOverLapping){
    collisionDirections typeCollision = NULLCOLLISION;
    bool isOver = mapCollider1['L'] + x1 <= mapCollider2['R']  &&  mapCollider1['R'] + x1 >= mapCollider2['L']  &&
                  mapCollider1['B'] + y1 <= mapCollider2['T']  &&
                  mapCollider1['T'] + y1 >= mapCollider2['B'];

    if(isOver){
        *quantityOverLapping += 1;
        if(mapCollider1['R'] + x1 > mapCollider2['L'] && mapCollider1['L'] + x1 < mapCollider2['L'] && mapCollider1['T'] + y1 - 0.1 > mapCollider2['B'] && mapCollider1['B'] + y1 + 0.1 < mapCollider2['T']){
            typeCollision = RIGHTCOLLISION;
        }
        else if(mapCollider1['L'] + x1 < mapCollider2['R'] && mapCollider1['R'] + x1 > mapCollider2['R'] && mapCollider1['T'] + y1 - 0.1 > mapCollider2['B'] && mapCollider1['B'] + y1 + 0.1 < mapCollider2['T']){
            typeCollision = LEFTCOLLISION;
        }
        if(mapCollider1['T'] + y1 > mapCollider2['B'] && mapCollider1['R'] + x1 - 0.1 > mapCollider2['L'] && mapCollider1['L'] + x1 + 0.1 < mapCollider2['R'] && mapCollider1['T'] + y1 < mapCollider2['T']){
            typeCollision = TOPCOLLISION;
        }
        else if(mapCollider1['B'] + y1 < mapCollider2['T'] && mapCollider1['R'] + x1 - 0.1 > mapCollider2['L'] && mapCollider1['L'] + x1 + 0.1 < mapCollider2['R'] && mapCollider1['B'] + y1 > mapCollider2['B']){
            typeCollision = BOTTOMCOLLISION;
        }
    }

    bool notOverlapping = *quantityOverLapping == 0;
    if (notOverlapping && lastIteration){
        typeCollision = NOCOLLISION;
    }

    return typeCollision;
};

#endif