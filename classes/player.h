#include "Object.h"

#ifndef PLAYER_H
#define PLAYER_H

enum Directions {UP, DOWN, LEFT, RIGHT};

class Player : public Object
{
    public: int life;
    public: float damage;
    public: int chargeShot;
    public: Directions direction = LEFT;
    public: map<char, double> mapColliderPlayer;
    public: void move(bool keyBuffer[256]);
    public: void drawPlayer(double playerX, double playerY, double playerZ, double playerSize, bool drawnCollider, double colliderSize);
    public: Player(){};
    public: Player(double x, double y, double z, float r, float g, float b, Speed speed, float size, int life, float damage, int chargeShot, Collision collision);
};

Player:: Player(double x, double y, double z, float r, float g, float b, Speed speed, float size, int life, float damage, int chargeShot, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
    this->life = life;
    this->damage = damage;
    this->chargeShot = chargeShot;
};

void Player:: move(bool keyBuffer[256]){
    if (this->speed.x != 0 && keyBuffer['d']) {
        this->x += this->speed.x;
    }

    if (this->speed.x != 0 && keyBuffer['a']) {
        this->x -= this->speed.x;
    }

    if (this->speed.y != 0){
        this->y += this->speed.y;
//        this->speed.y -= 0.001f;
    }

     if (this->collision.isOnPlataform == false){
         this->speed.y -= 0.001f;
     }

    // if (this->y < 0.0f) {
    //     this->speed.y = 0;
    //     this->y = 0.0f;
    // }
}

void Player:: drawPlayer(double playerX=0, double playerY=0, double playerZ=-6.5, double playerSize=0.5, bool drawnCollider=false, double colliderSize=1){
    //Player
    glPushMatrix();
        glTranslated(playerX, playerY, playerZ);
        glRotated(90, 1, 0, 0);
        glutSolidCube(playerSize);
    glPopMatrix();

    //Collision Cube
    if(drawnCollider){
        glPushMatrix();
            glTranslated(playerX, playerY, playerZ);
            glRotated(90, 1, 0, 0);
            glutWireCube(colliderSize);
        glPopMatrix();
    }
}

#endif
