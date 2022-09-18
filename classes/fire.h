#include "Object.h"

class Fire : public Object
{
    public: int slicesAndStacks = 16;
    public: void drawFire(double playerX, double playerY, bool drawnCollider);
    public: Fire()= default;
    public: Fire(double x, double y, double z, float r, float g, float b, Speed speed, float size, int slicesAndStacks, Collision collision) : Object(x, y, z, r, g, b, speed, size, collision){
        this->slicesAndStacks = slicesAndStacks;
    };
};

void Fire::drawFire(double playerX, double playerY, bool drawnCollider){
    glPushMatrix();
        glTranslated(this->x - playerX, this->y - playerY, this->z);
        glRotated(90, 1, 0, 0);
        glutSolidSphere(this->size, this->slicesAndStacks, this->slicesAndStacks);
    glPopMatrix();

    if(drawnCollider){
        glPushMatrix();
            glTranslated(this->collision.x + this->x - playerX, this->collision.y + this->y - playerY, this->collision.z);
            glRotated(90, 1, 0, 0);
            glutWireCube(this->collision.size + 0.3);
        glPopMatrix();
    }
    this->x += this->speed.x;
}