#include "Object.h"

class Player : public Object
{
    public: int life;
    public: float damage;
    public: int chargeShot;    
    public: void move(bool keyBuffer[256]);
    // public: Player(double x, double y, double z, float r, float g, float b, Speed speed, float radius, int slicesAndStacks);
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