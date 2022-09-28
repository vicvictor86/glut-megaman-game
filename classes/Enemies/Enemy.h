
#ifndef GAME_PROJECT_ENEMY_H
#define GAME_PROJECT_ENEMY_H
#include "Object.h"

class Enemy : public Object {
    public: int life = 3;
    public: int damage=1;
    public: int timeToShoot=-1;
    public: int shootColdDown=2;
    public: int coldDown=2;
    public: int timeToChangeDirection=-1;
    public: bool canTakeDamage=true;
    public: Collision viewOfEnemy;
    public: virtual void move();
    public: virtual void shoot(vector<Fire>* fireObjects);
    public: void getDamage(int takedDamage);
    public: void drawEnemy(const string &animationName, int animationFrame, double scaleSize, bool drawnCollider);
    public: virtual void noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, double sizeOfVision, bool drawnCollision);
    public: Enemy() = default;
};

void Enemy:: move(){
    if (this->speed.x != 0) {
        this->x += this->speed.x;
    }

    if (this->speed.y != 0){
        this->y += this->speed.y;
    }
}

void Enemy::shoot(vector<Fire>* fireObjects) {
    if (this->timeToShoot == -1) {
        this->timeToShoot = time(nullptr);
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToShoot > this->shootColdDown) {
        Fire fire;

        double spawnPoint = this->x + 1;
        double heightOfPlayer = this->y + 0;
        float shootSpeed = 0.06f;
        float radiusOfFire = 0.5;

        fire.x = spawnPoint;
        fire.y = heightOfPlayer;
        fire.z = this->z;

        fire.speed.x = shootSpeed;
        fire.sizeH = radiusOfFire;
        fire.collision.sizeH = 0.55;

        fire.slicesAndStacks = 16;
        fire.collision.x = 0;
        fire.collision.y = 0;
        fire.collision.z = fire.z;

        fire.tagShoot = "Enemy";
        fire.damage = this->damage;

        fireObjects->push_back(fire);
        this->timeToShoot = -1;
    }
}

void Enemy::getDamage(int takedDamage) {
    if(this->canTakeDamage){
        this->life -= takedDamage;
    }
}

void Enemy::noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, double sizeOfVision, bool drawnCollision) {
//    cout << "Enemy noticed" << endl;
}

void Enemy::drawEnemy(const string& animationName="", int animationFrame=1, double scaleSize=1, bool drawnCollider=false){
    //Collision Cube
    if(drawnCollider){
        glPushMatrix();
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

    //Player model
    glPushMatrix();
        glLoadIdentity();
        glTranslatef((float)this->x, (float)this->y, (float)this->z);
        glScaled(scaleSize, scaleSize, scaleSize);
        if(!this->animations[animationName].empty()){
            this->animations[animationName][animationFrame].draw();
        }
    glPopMatrix();
}

#endif //GAME_PROJECT_ENEMY_H
