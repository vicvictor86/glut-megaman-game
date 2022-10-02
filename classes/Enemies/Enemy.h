
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
    public: bool canShoot=false;
    public: double sizeVisionX = 2, sizeVisionY = 2;
    public: double offSetShootX = 1, offSetShootY = 0;
    public: string animationStatus = "idle";
    public: virtual void move();
    public: virtual void shoot(vector<Fire>* fireObjects, Player player, int actualFps);
    public: void getDamage(int takedDamage);
    public: void setSizeVision(double sizeVisionX, double sizeVisionY);
    public: void drawEnemy(const string &animationName, const Player& player, int animationFrame, double scaleSize, double rotationX, double rotationY, bool drawnCollider);
    public: virtual void noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, bool drawnCollision);
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

void Enemy::shoot(vector<Fire>* fireObjects, Player player, int actualFps) {
    if(!this->canShoot){
        return;
    }

    int actualTime = time(nullptr);
    if (actualTime - this->timeToShoot > this->shootColdDown || this->timeToShoot == -1) {
        Fire fire;

        double deltaX = player.x - this->x;
        double deltaY = player.y + player.collision.sizeV/2 - this->y;

        double offSetXDirectional = deltaX > 0 ? this->offSetShootX : -this->offSetShootX;
        double offSetYDirectional = deltaY > 0 ? this->offSetShootY : -this->offSetShootY;

        double spawnPointX = this->x + offSetXDirectional;
        double spawnPointY = this->y + offSetYDirectional;
        float radiusOfFire = 0.5;

        fire.x = spawnPointX;
        fire.y = spawnPointY;
        fire.z = this->z;



        fire.speed.x = deltaX/actualFps;
        fire.speed.y = deltaY/actualFps;
        fire.sizeH = radiusOfFire;
        fire.collision.sizeH = 0.55;

        fire.slicesAndStacks = 16;
        fire.collision.x = 0;
        fire.collision.y = 0;
        fire.collision.z = fire.z;

        fire.tagShoot = "Enemy";
        fire.damage = this->damage;

        fireObjects->push_back(fire);

        this->timeToShoot = this->timeToShoot == -1 ? time(nullptr) : -1;
    }
}

void Enemy::getDamage(int takedDamage) {
    if(this->canTakeDamage){
        this->life -= takedDamage;
    }
}

void Enemy::noticedEnemy(map<char, double> mapCollisionPlayer, double playerX, double playerY, double playerZ, bool drawnCollision) {
    map<char, double> mapCollisionoViewOfPlayer = Object::createRetangleCollider(this->collision.x, this->collision.y, playerZ, this->sizeVisionX);
    if(drawnCollision){
        glPushMatrix();
            Object::drawObject(this->x, this->y, this->z, this->sizeVisionX, this->sizeVisionY, 1, 0, 0);
        glPopMatrix();
    }

    int quantityOverLapping = 0;
    Collision::checkCollision(mapCollisionoViewOfPlayer, this->x, this->y, mapCollisionPlayer, playerX, playerY, true, &quantityOverLapping);
    this->canShoot = quantityOverLapping > 0;
}

void Enemy::drawEnemy(const string& animationName, const Player& player, int animationFrame=1, double scaleSize=1, double rotationX=0, double rotationY=0, bool drawnCollider=false){
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

    double deltaX = player.x - this->x;

    //Player model
    glPushMatrix();
        glLoadIdentity();
        glColor3d(1, 1 , 1);
        glTranslated(this->modelX + this->x, this->modelY + this->y, this->z);

        double angleRotate = deltaX >=0 ? 90 : -90;
        glRotated(angleRotate, 0, 1, 0);

        glScaled(scaleSize, scaleSize, scaleSize);
        if(!this->animations[animationName].empty()){
            this->animations[animationName][animationFrame].draw();
        }
    glPopMatrix();
}

void Enemy::setSizeVision(double updateSizeVisionX, double updateSizeVisionY=-1) {
    if(updateSizeVisionY == -1){
        updateSizeVisionY = updateSizeVisionX;
    }
    this->sizeVisionX = updateSizeVisionX;
    this->sizeVisionY = updateSizeVisionY;

}

#endif //GAME_PROJECT_ENEMY_H
