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
    public: void drawnPlayer(const string& animationName, int animationFrame, float scaleSize, bool drawnCollider, float r, float g, float b);
    public: void getDamage(int takedDamage);
    public: Player()= default;
    public: Player(float x, float y, float z, float r, float g, float b, Speed speed, float size, int life, int damage, int timeChargedShot, Collision collision);
};

Player:: Player(float x, float y, float z, float r, float g, float b, Speed speed, float size, int life, int damage, int timeChargedShot, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
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

void Player:: drawnPlayer(const string& animationName="", int animationFrame=1, float scaleSize=1, bool drawnCollider=false, float r=-1, float g=-1, float b=-1){
    r = (r == -1) ? this->r : r;
    g = (g == -1) ? this->g : g;
    b = (b == -1) ? this->b : b;
    glColor3d(r, g, b);

    //Collision Cube
    if(drawnCollider){
        glPushMatrix();
            glBegin(GL_LINE_LOOP);
                float xQuadLeft = this->collision.x - this->collision.sizeH / 2;
                float xQuadRight = this->collision.x + this->collision.sizeH / 2;
                float yQuadTop = this->collision.y + this->collision.sizeV / 2;
                float yQuadBottom = this->collision.y - this->collision.sizeV / 2;
                glVertex3d(xQuadLeft + this->x, yQuadBottom + this->y, this->collision.z);
                glVertex3d(xQuadRight + this->x, yQuadBottom + this->y, this->collision.z);
                glVertex3d(xQuadRight + this->x, yQuadTop + this->y, this->collision.z);
                glVertex3d(xQuadLeft + this->x, yQuadTop + this->y, this->collision.z);
            glEnd();
        glPopMatrix();
    }

    //Player model
    glPushMatrix();
        glLoadIdentity();
        glTranslatef((float)this->x, (float)this->y, (float)this->z);
        glRotatef(this->directionX == RIGHT ? 90 : -90, 0, 1, 0);
        glScaled(scaleSize, scaleSize, scaleSize);
        this->animations[animationName][animationFrame].draw();
    glPopMatrix();
}

void Player:: getDamage(int takedDamage){
    this->life -= takedDamage;
    Sounds::playSound("playerDamage");
}

#endif
