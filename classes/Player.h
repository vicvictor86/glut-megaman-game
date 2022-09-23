#include "Object.h"

#ifndef PLAYER_H
#define PLAYER_H

enum Directions {UP, DOWN, LEFT, RIGHT};

class Player : public Object
{
    public: int life=3;
    public: int damage=1;
    public: int timeChargedShot=3;
    public: Directions directionX = LEFT;
    public: Model model;
    public: void move(bool keyBuffer[256]);
    public: void drawnPlayer(bool drawnCollider);
    public: void getDamage(int takedDamage);
    public: Player()= default;
    public: Player(double x, double y, double z, float r, float g, float b, Speed speed, float size, int life, int damage, int timeChargedShot, Collision collision);
    public: void setModel(const string& path);
};

Player:: Player(double x, double y, double z, float r, float g, float b, Speed speed, float size, int life, int damage, int timeChargedShot, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
    this->life = life;
    this->damage = damage;
    this->timeChargedShot = timeChargedShot;
}

void Player:: move(bool keyBuffer[256]){
    if (this->speed.x != 0 && keyBuffer['d']) {
        this->x += this->speed.x;
    }

    if (this->speed.x != 0 && keyBuffer['a']) {
        this->x -= this->speed.x;
    }

    if (this->speed.y != 0){
        this->y += this->speed.y;
    }

     if (!this->collision.isOnPlataform){
         this->speed.y -= 0.001f;
     }
}

void Player:: drawnPlayer(bool drawnCollider=false){
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
        glRotatef(90, 0, 1.0f, 0.0f);
        glScalef(0.3, 0.3, 0.3);
        this->model.draw();
    glPopMatrix();
}

void Player:: setModel(const string& path){
    this->model.load(path.c_str());
}

void Player:: getDamage(int takedDamage){
    this->life -= takedDamage;
}

#endif
