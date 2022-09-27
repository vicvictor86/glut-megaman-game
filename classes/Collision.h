#include <map>
using namespace std;

#ifndef COLLISION_H
#define COLLISION_H

enum collisionDirections {TOPCOLLISION, BOTTOMCOLLISION, LEFTCOLLISION, RIGHTCOLLISION, NOCOLLISION, NULLCOLLISION};

class Collision {
    public: float x=0, y=0, z=0;
    public: float r=0, g=0, b=0;
    public: float sizeH=1, sizeV=1;
    public: bool isOnPlataform = false;
    public: Collision()= default;
    public: Collision(float x, float y, float z, float sizeH, float sizeV);
    public: void setSize(float sizeH, float sizeV);
    public: static collisionDirections checkCollision(map<char, float> mapCollider1, float x1, float y1, map<char, float> mapCollider2, float x2, float y2, bool lastIteration, int* quantityOverLapping);
};

Collision :: Collision(float x, float y, float z, float sizeH, float sizeV=-1){
    if(sizeV == -1){
        sizeV = sizeH;
    }
    this->x = x;
    this->y = y;
    this->z = z;
    this->sizeH = sizeH;
    this->sizeV = sizeV;
}

void Collision :: setSize(float updateSizeH, float updateSizeV=-1){
    if(updateSizeV == -1){
        updateSizeV = updateSizeH;
    }
    this->sizeH = updateSizeH;
    this->sizeV = updateSizeV;
}

collisionDirections Collision :: checkCollision(map<char, float> mapCollider1 = map<char, float>(), float x1=0, float y1=0, map<char, float> mapCollider2 = map<char, float>(), float x2=0, float y2=0, bool lastIteration=false, int* quantityOverLapping=nullptr){
    collisionDirections typeCollision = NULLCOLLISION;
    bool isOver = mapCollider1['L'] + x1 <= mapCollider2['R'] + x2  &&  mapCollider1['R'] + x1 >= mapCollider2['L'] + x2  &&
                  mapCollider1['B'] + y1 <= mapCollider2['T'] + y2  &&
                  mapCollider1['T'] + y1 >= mapCollider2['B'] + y2;

    if(isOver){
        *quantityOverLapping += 1;
        if(mapCollider1['R'] + x1 > mapCollider2['L'] + x2 && mapCollider1['L'] + x1 < mapCollider2['L'] + x2 && mapCollider1['T'] + y1 - 0.1 > mapCollider2['B'] + y2 && mapCollider1['B'] + y1 + 0.1 < mapCollider2['T'] + y2){
            typeCollision = RIGHTCOLLISION;
        }
        else if(mapCollider1['L'] + x1 < mapCollider2['R'] + x2 && mapCollider1['R'] + x1 > mapCollider2['R'] + x2 && mapCollider1['T'] + y1 - 0.1 > mapCollider2['B'] + y2 && mapCollider1['B'] + y1 + 0.1 < mapCollider2['T'] + y2){
            typeCollision = LEFTCOLLISION;
        }
        if(mapCollider1['T'] + y1 > mapCollider2['B'] + y2 && mapCollider1['R'] + x1 - 0.1 > mapCollider2['L'] + x2 && mapCollider1['L'] + x1 + 0.1 < mapCollider2['R'] + x2 && mapCollider1['T'] + y1 < mapCollider2['T'] + y2){
            typeCollision = TOPCOLLISION;
        }
        else if(mapCollider1['B'] + y1 < mapCollider2['T'] + y2 && mapCollider1['R'] + x1 - 0.1 > mapCollider2['L'] + x2 && mapCollider1['L'] + x1 + 0.1 < mapCollider2['R'] + x2 && mapCollider1['B'] + y1 > mapCollider2['B'] + y2){
            typeCollision = BOTTOMCOLLISION;
        }
    }

    bool notOverlapping = *quantityOverLapping == 0;
    if (notOverlapping && lastIteration){
        typeCollision = NOCOLLISION;
    }

    return typeCollision;
}

#endif