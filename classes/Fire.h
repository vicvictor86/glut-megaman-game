#include "Object.h"

class Fire : public Object
{
    public: int slicesAndStacks = 16;
    public: bool chargedFire = false;
    public: int damage = 1;
    public: string tagShoot;
    public: int timeToLive = 2;
    public: int timeLiving = -1;
    public: void drawFire(bool drawnCollider);
    public: map<char, double> mapCollider;
    public: bool isAlive();
    public: Fire()= default;
    public: Fire(double x, double y, double z, float r, float g, float b, Speed speed, float size, int slicesAndStacks, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
        this->slicesAndStacks = slicesAndStacks;
    };
};

void Fire::drawFire(bool drawnCollider){
    glPushMatrix();
        glColor3d(1, 0, 0);
        glTranslated(this->x, this->y, this->z);
        glRotated(90, 1, 0, 0);
        glutSolidSphere(this->sizeH, this->slicesAndStacks, this->slicesAndStacks);
    glPopMatrix();

    if(drawnCollider){
        glPushMatrix();
            glColor3d(0, 0, 0);
            glTranslated(this->collision.x + this->x, this->collision.y + this->y, this->collision.z);
            glRotated(90, 1, 0, 0);
            glutWireCube(this->collision.sizeH + 0.3);
        glPopMatrix();
    }

    this->x += this->speed.x;
    this->y += this->speed.y;
}

bool Fire::isAlive(){
    if(this->timeLiving == -1){
        this->timeLiving = time(nullptr);
    }

    int actualTime = time(nullptr);
    if(actualTime - this->timeLiving > this->timeToLive){
        return false;
    }

    return true;
}