#ifndef GAME_PROJECT_WALL_H
#define GAME_PROJECT_WALL_H

#include "Object.h"

class Wall : public Object{
    public: map<char, double> mapColliderWall;
    public: virtual void move();
    public: Wall() = default;
    public: Wall(double x, double y, double z, map<char, double> mapColliderWall);

    public: void drawWall(const string &animationName, int animationFrame, double scaleSizeH, bool drawnCollider, bool devMode);
};

void Wall::move() {}

Wall::Wall(double x, double y, double z, map<char, double> mapColliderWall) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->mapColliderWall = mapColliderWall;
}

void Wall:: drawWall(const string& animationName="", int animationFrame=1, double scaleModel=1, bool drawnCollider=false, bool devMode=false){
    if(drawnCollider){
        Object::drawObject(this->x, this->y, this->z, this->sizeH, this->sizeV);
    }

    glPushMatrix();
        glLoadIdentity();
        glColor3d(1, 1 , 1);
        glTranslated(this->modelX + this->x, this->modelY + this->y, this->z);
        glRotated(90, 0, 1, 0);

        glScaled(scaleModel, scaleModel, scaleModel);
        if(!this->animations[animationName].empty()){
            this->animations[animationName][animationFrame].draw();
        }
    glPopMatrix();
}

#endif //GAME_PROJECT_WALL_H
