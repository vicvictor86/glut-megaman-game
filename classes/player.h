#include "Object.h"

class Player : public Object
{
    public: int life;
    public: float damage;
    public: int chargeShot;    
    public: void move(bool keyBuffer[256]);
    public: Player(){};
    public: Player(double x, double y, double z, float r, float g, float b, Speed speed, float radius, int life, float damage, int chargeShot);
};

Player:: Player(double x, double y, double z, float r, float g, float b, Speed speed, float radius, int life, float damage, int chargeShot) : Object(x, y, z, r, g, b, speed, radius){
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
        this->speed.y -= 0.001f;
    }

    if (this->y < 0.0f) {
        this->speed.y = 0;
        this->y = 0.0f;
    }
}