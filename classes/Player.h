#include "Object.h"
#include "Sounds.h"

#ifndef PLAYER_H
#define PLAYER_H

enum Directions {UP, DOWN, LEFT, RIGHT};

class Player : public Object
{
    public: int life=3;
    public: int maxLife=3;
    public: int damage=1;
    public: int timeChargedShot=3;
    public: bool isShooting=false;
    public: Directions directionX = RIGHT;
    public: void move(bool keyBuffer[256]);
    public: int drawPlayer(const string& animationName, int animationFrame, double scaleSize, bool drawnCollider, bool devMode);
    public: void getDamage(int takedDamage);
    public: Player()= default;
    public: Player(double x, double y, double z, float r, float g, float b, Speed speed, float size, int life, int damage, int timeChargedShot, Collision collision);
};

Player:: Player(double x, double y, double z, float r, float g, float b, Speed speed, float size, int life, int damage, int timeChargedShot, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
    this->life = life;
    this->damage = damage;
    this->timeChargedShot = timeChargedShot;
    this->maxLife = life;
}

void Player:: move(bool keyBuffer[256]){
    if (this->speed.x != 0 && (keyBuffer['d'] || keyBuffer['D'])) {
        this->x += this->speed.x;
    }

    if (this->speed.x != 0 && (keyBuffer['a'] || keyBuffer['A'])) {
        this->x -= this->speed.x;
    }

    if (this->speed.y != 0){
        this->y += this->speed.y;
    }

     if (!this->collision.isOnPlataform){
         this->speed.y -= 0.001f;
     }

    if (!keyBuffer['a'] && !keyBuffer['A'] && !keyBuffer['d'] && !keyBuffer['D']) {
        this->speed.x = 0;
    }
}

int Player:: drawPlayer(const string& animationName="", int animationFrame=1, double scaleSize=1, bool drawnCollider=false, bool devMode=false){
    //Collision Cube
    if(drawnCollider){
        glPushMatrix();
            glColor3d(0, 0, 0);
            glBegin(GL_LINE_LOOP);
                double xQuadLeft = this->collision.x - this->collision.sizeH / 2;
                double xQuadRight = this->collision.x + this->collision.sizeH / 2;
                double yQuadTop = this->collision.y + this->collision.sizeV / 2;
                double yQuadBottom = this->collision.y - this->collision.sizeV / 2;
                glVertex3d(xQuadLeft + this->x, yQuadBottom + this->y, this->collision.z);
                glVertex3d(xQuadRight + this->x, yQuadBottom + this->y, this->collision.z);
                glVertex3d(xQuadRight + this->x, yQuadTop + this->y, this->collision.z);
                glVertex3d(xQuadLeft + this->x, yQuadTop + this->y, this->collision.z);
            glEnd();
        glPopMatrix();
    }

    if(!devMode){
        //Player model
        glPushMatrix();
            glColor3d(this->r, this->g, this->b);
            glLoadIdentity();
            glTranslatef((float)this->x, (float)this->y, (float)this->z);
            glRotatef(this->directionX == RIGHT ? 90 : -90, 0, 1, 0);
            glScaled(scaleSize, scaleSize, scaleSize);
            if(!this->animations[animationName].empty()){
                if(animationFrame > this->animations[animationName].size()){
                    animationFrame = 0;
                }
                this->animations[animationName][animationFrame].draw();
            } else {
//                cout << "A animacao " << animationName << " nao existe" << endl;
            }
        glPopMatrix();
    }

    return animationFrame;
}

void Player:: getDamage(int takedDamage){
    this->life -= takedDamage;
    Sounds::playSound("playerDamage");
}

#endif
