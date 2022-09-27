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
    public: void drawnPlayer(double scaleSize, bool drawnCollider, double r, double g, double b);
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
}

void Player:: drawnPlayer(double scaleSize=1, bool drawnCollider=false, double r=-1, double g=-1, double b=-1){
    r = (r == -1) ? this->r : r;
    g = (g == -1) ? this->g : g;
    b = (b == -1) ? this->b : b;
    glColor3d(r, g, b);
    //Player
    glPushMatrix();
        glTranslated(this->x, this->y, this->z);
        glutSolidCube(this->size);
    glPopMatrix();

    //Collision Cube
    if(drawnCollider){
        glPushMatrix();
            glTranslated(this->x, this->y, this->z);
            glutWireCube(this->collision.size);
        glPopMatrix();
    }

    glPushMatrix();
        glLoadIdentity();
        glTranslatef((float)this->x, (float)this->y, (float)this->z);
        if(this->directionX == RIGHT){
            glRotatef(90, 0, 1, 0);
        }else{
            glRotatef(-90, 0, 1, 0);
        }
        glScalef(scaleSize, scaleSize, scaleSize);
        this->model.draw();
    glPopMatrix();
}

void Player:: getDamage(int takedDamage){
    this->life -= takedDamage;
    Sounds::playSound("playerDamage");
}

#endif
